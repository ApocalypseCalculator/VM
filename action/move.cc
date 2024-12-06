#include "move.h"
#include "../model/vmstate.h"
#include "../model/cursor.h"
#include <ncurses.h>

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
    }
    vmstate->getController()->flushBuffer();
}

bool MoveCursor::matchAction(const std::vector<int> &input) {
    if(input.size() == 1) {
        if(input.at(0) == KEY_UP || input.at(0) == KEY_DOWN || input.at(0) == KEY_LEFT || input.at(0) == KEY_RIGHT) {
            return true;
        }
        if(input.at(0) == 'h' || input.at(0) == 'j' || input.at(0) == 'k' || input.at(0) == 'l') {
            return true;
        }
    }
    return false;
}

MoveCursor::~MoveCursor() = default;
