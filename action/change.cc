#include "change.h"
#include "../model/vmstate.h"

// #define ctrl(x) ((x) & 0x1f)

ChangeMode::ChangeMode() {
    mode = Mode::NORMAL;
}

// for changing mode to Insert or Command (colon commands)

// valid single char inputs
static const int validInputs[] = {
    'a', 
    'i', 
    'I',
    'A',
    'o',
    'O',
    's',
    'S',
    'r',
    'R',
    ':'
};

void ChangeMode::doAction(const std::vector<int> &input, VMState *vmstate) {
    // vmstate->insert(input);
    if(input.at(0) == 'a') {
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getFileState()->moveCursor(1, 0, false);
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'i') {
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'I') {
        // copied from move.cc's ^ command (seek first nonwhitespace)
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
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'A') {
        // go to end first
        Cursor search = vmstate->getFileState()->getCursor();
        std::string line = vmstate->getFileState()->getLine(search.lineidx);
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, static_cast<int>(line.size())});
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'o') {
        // go to end first, then insert \n (cursor moves automatically)
        Cursor search = vmstate->getFileState()->getCursor();
        std::string line = vmstate->getFileState()->getLine(search.lineidx);
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, static_cast<int>(line.size())});
        vmstate->getFileState()->insert("\n");
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'O') {
        // go to start first, insert \n, move cursor up
        Cursor search = vmstate->getFileState()->getCursor();
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, 0});
        vmstate->getFileState()->insert("\n");
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, 0}); // this should now be the prev line
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 's') {
        // removechar, then insert
        vmstate->getFileState()->moveCursor(1, 0, false);
        vmstate->getFileState()->removeChar();
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'S') {
        // remove line, then insert
        Cursor search = vmstate->getFileState()->getCursor();
        vmstate->getFileState()->removeLine(search.lineidx);
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, 0});
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    }
    else if(input.at(0) == 'r') {
        // r does not cause cmd bar changes
        vmstate->getCommandBarState()->setCommandBar("");
        vmstate->getController()->setMode(Mode::REPLACE);
    }
    else if(input.at(0) == 'R') {
        vmstate->getCommandBarState()->setCommandBar("-- REPLACE --");
        vmstate->getController()->setMode(Mode::REPLACE);
    }
    else if(input.at(0) == ':') {
        vmstate->getCommandBarState()->setCommandBar(":");
        vmstate->getController()->setMode(Mode::COMMAND);
        vmstate->getController()->flushBuffer();
    }
    vmstate->getController()->flushBuffer();
}

bool ChangeMode::matchAction(const std::vector<int> &input) {
    if(input.size() == 1) {
        for(auto &i : validInputs) {
            if(input.at(0) == i) {
                return true;
            }
        }
    }
    return false;
}

ChangeMode::~ChangeMode() = default;
