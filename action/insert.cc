#include "insert.h"
#include "../model/vmstate.h"
#include <ncurses.h>

Insertion::Insertion() {
    mode = Mode::INSERT;
}

void Insertion::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.size() == 1) {
        if(input.at(0) == KEY_UP) {
            vmstate->getFileState()->moveCursor(0, -1);
        } else if(input.at(0) == KEY_DOWN) {
            vmstate->getFileState()->moveCursor(0, 1);
        } else if(input.at(0) == KEY_LEFT) {
            vmstate->getFileState()->moveCursor(-1, 0);
        } else if(input.at(0) == KEY_RIGHT) {
            vmstate->getFileState()->moveCursor(1, 0);
        }
        else if(input.at(0) == 27) { // escape key
            vmstate->getCommandBarState()->setCommandBar("");
            vmstate->getController()->setMode(Mode::NORMAL);
        }
        else if(input.at(0) == 127) { // backspace key
            vmstate->getFileState()->removeChar();
        }
        else {
            vmstate->getFileState()->insertChar(input.at(0));
        }
    }
    vmstate->getController()->flushBuffer();
}

bool Insertion::matchAction(const std::vector<int> &input) {
    return true;
}

Insertion::~Insertion() = default;
