#include "replace.h"
#include "../model/vmstate.h"

Replace::Replace() {
    mode = Mode::REPLACE;
}

void Replace::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.size() == 1) {
        int givenCh = input.at(0);
        // disable cursor navigation in replace mode
        /*if(givenCh == KEY_UP) {
            vmstate->getFileState()->moveCursor(0, -1, false);
        } else if(givenCh == KEY_DOWN) {
            vmstate->getFileState()->moveCursor(0, 1, false);
        } else if(givenCh == KEY_LEFT) {
            vmstate->getFileState()->moveCursor(-1, 0, false);
        } else if(givenCh == KEY_RIGHT) {
            vmstate->getFileState()->moveCursor(1, 0, false);
        }
        else */
        if(givenCh == 27) { // escape key
            vmstate->getCommandBarState()->setCommandBar("");
            vmstate->getController()->setMode(Mode::NORMAL);
            vmstate->getHistory()->createChange(vmstate);
            if(vmstate->getController()->getModeMultiplier() > 1) {
                vmstate->getController()->setReplay(std::string(vmstate->getController()->getModeMultiplier()-1, '.'));
            }
        }
        else {
            Cursor curcursor = vmstate->getFileState()->getCursor();
            if(curcursor.charidx < vmstate->getFileState()->getLine(curcursor.lineidx).size()) {
                vmstate->getFileState()->moveCursor(1, 0, false);
                vmstate->getFileState()->removeChar();
            }
            vmstate->getFileState()->insertChar(givenCh);
        }
    }
    // this means that 'r' was pressed, and not 'R', so we return
    if(vmstate->getCommandBarState()->getCommandBar().size() == 0) {
        vmstate->getController()->setMode(Mode::NORMAL);
        vmstate->getHistory()->createChange(vmstate);
    }
    vmstate->getController()->flushBuffer();
}

bool Replace::matchAction(const std::vector<int> &input) {
    return true;
}

Replace::~Replace() = default;
