#include "move.h"
#include "../model/vmstate.h"
#include "../model/cursor.h"
#include <ncurses.h>
#include "../model/cursor.h"

#define ctrl(x) ((x) & 0x1f)

MoveCursor::MoveCursor() {
    mode = Mode::NORMAL;
}

// valid single char inputs
static const int validInputs[] = {
    KEY_UP, 
    KEY_DOWN, 
    KEY_LEFT, 
    KEY_RIGHT, 
    'h', 
    'j', 
    'k', 
    'l', 
    'w', 
    'b', 
    '^', 
    '$', 
    '0', 
    ';', 
    ctrl('b'), 
    ctrl('f'),
    ctrl('u'),
    ctrl('d'), 
    '%',
    'J'
};

bool isWhiteSpace(char c) {
    return c == ' ' || c == '\t';
}

void MoveCursor::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.at(0) == KEY_UP || input.at(0) == 'k') {
        vmstate->getFileState()->moveCursor(0, -1, true);
    } else if(input.at(0) == KEY_DOWN || input.at(0) == 'j') {
        vmstate->getFileState()->moveCursor(0, 1, true);
    } else if(input.at(0) == KEY_LEFT || input.at(0) == 'h') {
        vmstate->getFileState()->moveCursor(-1, 0, true);
    } else if(input.at(0) == KEY_RIGHT || input.at(0) == 'l') {
        vmstate->getFileState()->moveCursor(1, 0, true);
    } else if(input.at(0) == 'w') {
        FileState *file = vmstate->getFileState();
        Cursor search = file->getCursor();
        bool foundws = false;
        bool newlined = false;
        while(search.lineidx < file->getLineCount()) {
            std::string line = file->getLine(search.lineidx);
            char nextchar;
            // finding next character within line
            if(search.charidx+1 < line.size()) {
                search.charidx++;
                nextchar = line.at(search.charidx);
            } else {
                if(newlined) {
                    break;
                }
                // newline
                // last character of last line
                if(search.lineidx == file->getLineCount()-1) {
                    if(search.charidx < line.size()) search.charidx++; // go to last character
                    break;
                }
                search.charidx = 0;
                search.lineidx++;
                foundws = true; // found a newline
                newlined = true;
                if(file->getLine(search.lineidx).size() > 0) {
                    nextchar = file->getLine(search.lineidx).at(0);
                }
            }
            if(nextchar == ' ' || nextchar == '\t') {
                foundws = true;
            } else if(foundws) {
                // we reached the first character after whitespace
                break;
            }
        }
        file->setCursor(search);
    }  else if(input.at(0) == 'b') {
        FileState *file = vmstate->getFileState();
        Cursor search = file->getCursor();
        bool foundws = false;
        bool newlined = false;
        if(search.charidx == 0 || isWhiteSpace(file->getLine(search.lineidx).at(search.charidx-1))) {
            // then we seek past the whitespace
        while(search.lineidx >= 0) {
            std::string line = file->getLine(search.lineidx);
            char prevchar;
            // finding next character within line
            if(search.charidx > 0) {
                search.charidx--;
                prevchar = line.at(search.charidx);
            } else {
                if(newlined) {
                    break;
                }
                // newline
                // first character of first line
                if(search.lineidx == 0) {
                    search.charidx = 0; // go to first character
                    break;
                }
                search.lineidx--;
                search.charidx = file->getLine(search.lineidx).size()-1; // NOTE: this could set to -1
                foundws = true; // found a newline
                newlined = true;
                if(file->getLine(search.lineidx).size() > 0) {
                    prevchar = file->getLine(search.lineidx).back();
                }
            }
            if(prevchar == ' ' || prevchar == '\t') {
                foundws = true;
            } else if(foundws) {
                break;
            }
        }
        // we reached the first character before first whitespace
        // we just need to seek backwards until we find another 
        // whitespace OR we reach the beginning of the line
        if(search.charidx > 0) {
            std::string line = file->getLine(search.lineidx);
            while(search.charidx > 0) {
                search.charidx--;
                if(line.at(search.charidx) == ' ' || line.at(search.charidx) == '\t') {
                    search.charidx++;
                    break;
                }
            }
        } else {
            search.charidx = 0;
        }
        }
        else { // here, we are clearly not the start of a word, so we simply find the start
            while(search.charidx > 0) {
                search.charidx--;
                if(isWhiteSpace(file->getLine(search.lineidx).at(search.charidx))) {
                    search.charidx++;
                    break;
                }
            }
        }
        file->setCursor(search);
    } else if(input.at(0) == '^') {
        Cursor search = vmstate->getFileState()->getCursor();
        std::string line = vmstate->getFileState()->getLine(search.lineidx);
        int charidx = 0;
        for(auto &c : line) {
            if(c == ' ' || c == '\t') {
                charidx++;
            } else {
                break;
            }
        }
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, charidx});
    } else if(input.at(0) == '$') {
        Cursor search = vmstate->getFileState()->getCursor();
        // static cast unsigned long (size_t) to int
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, static_cast<int>(vmstate->getFileState()->getLine(search.lineidx).size())});
    } else if(input.at(0) == '0') {
        Cursor search = vmstate->getFileState()->getCursor();
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, 0});
    } else if(input.at(0) == ';') {
        char searchChar = vmstate->getCommandBarState()->getSearchChar();
        if(searchChar != '\0') { // default search (no search)
            bool searchFwd = vmstate->getCommandBarState()->getSearchForward();
            // copy pasted and modified from `f` section below
            Cursor search = vmstate->getFileState()->getCursor();
            std::string line = vmstate->getFileState()->getLine(search.lineidx);
            if(searchFwd) {
                for(int i = search.charidx+1; i < line.size(); i++) {
                    if(line.at(i) == searchChar) {
                        vmstate->getFileState()->setCursor(Cursor{search.lineidx, i});
                        break;
                    }
                }
            } else {
                for(int i = search.charidx-1; i >= 0; i--) {
                    if(line.at(i) == searchChar) {
                        vmstate->getFileState()->setCursor(Cursor{search.lineidx, i});
                        break;
                    }
                }
            }
        }
    } else if(input.size() == 2) {
        Cursor search = vmstate->getFileState()->getCursor();
        std::string line = vmstate->getFileState()->getLine(search.lineidx);
        if(input.at(0) == 'f') {
            for(int i = search.charidx+1; i < line.size(); i++) {
                if(line.at(i) == input.at(1)) {
                    vmstate->getFileState()->setCursor(Cursor{search.lineidx, i});
                    break;
                }
            }
            vmstate->getCommandBarState()->setSearch(input.at(1));
            vmstate->getCommandBarState()->setSearchForward(true);
        } else if(input.at(0) == 'F') {
            for(int i = search.charidx-1; i >= 0; i--) {
                if(line.at(i) == input.at(1)) {
                    vmstate->getFileState()->setCursor(Cursor{search.lineidx, i});
                    break;
                }
            }
            vmstate->getCommandBarState()->setSearch(input.at(1));
            vmstate->getCommandBarState()->setSearchForward(false);
        }
    } else if (input.at(0) == ctrl('b')) {
        // move up 1 page
        vmstate->getFileState()->setForcedPageMove(-2);
    } else if (input.at(0) == ctrl('f')) {
        // move down 1 page
        vmstate->getFileState()->setForcedPageMove(2);
    } else if (input.at(0) == ctrl('u')) {
        // move up 1 page
        vmstate->getFileState()->setForcedPageMove(-1);
    } else if (input.at(0) == ctrl('d')) {
        // move down 1/2 page
        vmstate->getFileState()->setForcedPageMove(1);
    } else if (input.at(0) == '%') {
        // move to matching bracket
        Cursor search = vmstate->getFileState()->getCursor();
        vmstate->getFileState()->setCursor(findMatch(search, vmstate->getFileState()));
    } else if (input.at(0) == 'J') {
        // join lines
        Cursor search = vmstate->getFileState()->getCursor();
        if(search.lineidx < vmstate->getFileState()->getLineCount()-1) {
            std::string line = vmstate->getFileState()->getLine(search.lineidx);
            std::string nextline = vmstate->getFileState()->getLine(search.lineidx+1);
            // remove leading whitespace
            while(nextline.size() > 0 && (nextline.at(0) == ' ' || nextline.at(0) == '\t')) {
                nextline = nextline.substr(1);
            }
            vmstate->getFileState()->replaceLine(search.lineidx, line + " " + nextline);
            vmstate->getFileState()->removeLine(search.lineidx+1);
        }
    }
    vmstate->getController()->flushBuffer();
}

bool MoveCursor::matchAction(const std::vector<int> &input) {
    if(input.size() == 1) {
        for(auto &i : validInputs) {
            if(input.at(0) == i) {
                return true;
            }
        }
    }
    else if(input.size() == 2 && (input.at(0) == 'f' || input.at(0) == 'F')) {
        return true;
    }
    return false;
}

MoveCursor::~MoveCursor() = default;

Cursor MoveCursor::findMatch(Cursor start, FileState *file) {
    // only match on valid depth 0 (every nested is +1 or -1)
    int depth = -1;
    // current search cursor
    Cursor search = start;
    // direction of search (fwd or bwd)
    bool fwd = true;
    std::string curLine = file->getLine(search.lineidx);
    if(curLine.size() <= search.charidx) {
        return start;
    }
    char searchChar = curLine.at(search.charidx);
    char opposite;
    if(searchChar == '(') {
        opposite = ')';
    } else if(searchChar == '[') {
        opposite = ']';
    } else if(searchChar == '{') {
        opposite = '}';
    } else if(searchChar == ')') {
        fwd = false;
        opposite = '(';
    } else if(searchChar == ']') {
        fwd = false;
        opposite = '[';
    } else if(searchChar == '}') {
        fwd = false;
        opposite = '{';
    } else {
        // not a bracket
        return start;
    }

    if(fwd) {
        while(search.lineidx < file->getLineCount()) {
            std::string line = file->getLine(search.lineidx);
            if(line.size() == 0) {
                search.charidx = 0;
                search.lineidx++;
                continue;
            }
            if(search.charidx < line.size()) {
                if(line.at(search.charidx) == searchChar) {
                    depth++;
                } else if(line.at(search.charidx) == opposite) {
                    if(depth == 0) {
                        return search;
                    } else {
                        depth--;
                    }
                }
            }
            search.charidx++;
            if(search.charidx == line.size()) {
                search.charidx = 0;
                search.lineidx++;
            }
        }
    }
    else {
        while(search.lineidx >= 0) {
            std::string line = file->getLine(search.lineidx);
            if(line.size() == 0) {
                search.charidx = file->getLine(search.lineidx).size()-1;
                search.lineidx--;
                if(search.charidx < 0) {
                    search.charidx = 0;
                }
                continue;
            }
            if(search.charidx >= 0) {
                if(line.at(search.charidx) == searchChar) {
                    depth++;
                } else if(line.at(search.charidx) == opposite) {
                    if(depth == 0) {
                        return search;
                    } else {
                        depth--;
                    }
                }
            }
            search.charidx--;
            if(search.charidx < 0) {
                search.lineidx--;
                if(search.lineidx >= 0) {
                    search.charidx = file->getLine(search.lineidx).size()-1;
                    if(search.charidx < 0) {
                        search.charidx = 0;
                    }
                }
            }
        }
    }
    // not found, return original (dont change cursor)
    return start;
}
