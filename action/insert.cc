#include "insert.h"
#include "../model/vmstate.h"
#include <ncurses.h>

Insertion::Insertion() {
    mode = Mode::INSERT;
}

void Insertion::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.size() == 1) {
        int givenCh = input.at(0);
        if(givenCh == KEY_UP) {
            vmstate->getFileState()->moveCursor(0, -1, false);
        } else if(givenCh == KEY_DOWN) {
            vmstate->getFileState()->moveCursor(0, 1, false);
        } else if(givenCh == KEY_LEFT) {
            vmstate->getFileState()->moveCursor(-1, 0, false);
        } else if(givenCh == KEY_RIGHT) {
            vmstate->getFileState()->moveCursor(1, 0, false);
        }
        else if(givenCh == 27) { // escape key
            vmstate->getCommandBarState()->setCommandBar("");
            vmstate->getController()->setMode(Mode::NORMAL);
            // move cursor back by 1
            vmstate->getFileState()->moveCursor(-1, 0, true);
        }
        else if(givenCh == 127 || givenCh == KEY_BACKSPACE || givenCh == KEY_DC) { // backspace key
            vmstate->getFileState()->removeChar();
        }
        else {
            vmstate->getFileState()->insertChar(givenCh);
        }
    }
    vmstate->getController()->flushBuffer();
}

bool Insertion::matchAction(const std::vector<int> &input) {
    return true;
}

Insertion::~Insertion() = default;
