#include "change.h"
#include "../model/vmstate.h"

// #define ctrl(x) ((x) & 0x1f)

ChangeMode::ChangeMode() {
    mode = Mode::NORMAL;
    copypaste = std::make_unique<CopyPaste>();
    selfdefmultiply = true;
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
    ':',
    '/',
    '?'
};

void ChangeMode::doAction(const std::vector<int> &input, VMState *vmstate, int multiplier) {
    // vmstate->insert(input);
    if(input.at(0) == 'a') {
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getFileState()->moveCursor(1, 0, false);
        vmstate->getController()->setMode(Mode::INSERT);
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == 'i') {
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
        vmstate->getHistory()->setRecentEditCmd(input);
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
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == 'A') {
        // go to end first
        Cursor search = vmstate->getFileState()->getCursor();
        std::string line = vmstate->getFileState()->getLine(search.lineidx);
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, static_cast<int>(line.size())});
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == 'o') {
        // go to end first, then insert \n (cursor moves automatically)
        Cursor search = vmstate->getFileState()->getCursor();
        std::string line = vmstate->getFileState()->getLine(search.lineidx);
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, static_cast<int>(line.size())});
        vmstate->getFileState()->insertChar('\n');
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == 'O') {
        // go to start first, insert \n, move cursor up
        Cursor search = vmstate->getFileState()->getCursor();
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, 0});
        vmstate->getFileState()->insertChar('\n');
        vmstate->getFileState()->setCursor(Cursor{search.lineidx, 0}); // this should now be the prev line
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == 's') {
        // alias for cl
        copypaste->doAction(std::vector<int>{'c', 'l'}, vmstate, multiplier);
    }
    else if(input.at(0) == 'S') {
        // alias for cc
        copypaste->doAction(std::vector<int>{'c', 'c'}, vmstate, multiplier);
    }
    else if(input.at(0) == 'r') {
        // r does not cause cmd bar changes
        vmstate->getCommandBarState()->setCommandBar("");
        vmstate->getController()->setMode(Mode::REPLACE);
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == 'R') {
        vmstate->getCommandBarState()->setCommandBar("-- REPLACE --");
        vmstate->getController()->setMode(Mode::REPLACE);
        vmstate->getHistory()->setRecentEditCmd(input);
    }
    else if(input.at(0) == ':') {
        vmstate->getCommandBarState()->setCommandBar(":");
        vmstate->getController()->setMode(Mode::COMMAND);
        return; // NOTE: do not flush buffer here, we need to know : to determine command
    } else if(input.at(0) == '/' || input.at(0) == '?') {
        vmstate->getCommandBarState()->setCommandBar("");
        vmstate->getCommandBarState()->appendCommandBar(input.at(0));
        vmstate->getController()->setMode(Mode::COMMAND);
        // note 2: since we don't flush the buffer, multiplier is preserved
        return; // NOTE: do not flush buffer here, we need to know / or ? to determine search direction
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
