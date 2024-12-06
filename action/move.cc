#include "move.h"
#include "../model/vmstate.h"
#include "../model/cursor.h"
#include <ncurses.h>
#include "../model/cursor.h"

MoveCursor::MoveCursor() {
    mode = Mode::NORMAL;
}

void MoveCursor::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.at(0) == KEY_UP || input.at(0) == 'k') {
        vmstate->getFileState()->moveCursor(0, -1);
    } else if(input.at(0) == KEY_DOWN || input.at(0) == 'j') {
        vmstate->getFileState()->moveCursor(0, 1);
    } else if(input.at(0) == KEY_LEFT || input.at(0) == 'h') {
        vmstate->getFileState()->moveCursor(-1, 0);
    } else if(input.at(0) == KEY_RIGHT || input.at(0) == 'l') {
        vmstate->getFileState()->moveCursor(1, 0);
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
    }
    vmstate->getController()->flushBuffer();
}

// valid single char inputs
static const int validInputs[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 'h', 'j', 'k', 'l', 'w', '^', '$', '0'};

bool MoveCursor::matchAction(const std::vector<int> &input) {
    if(input.size() == 1) {
        for(auto &i : validInputs) {
            if(input.at(0) == i) {
                return true;
            }
        }
    }
    return false;
}

MoveCursor::~MoveCursor() = default;
