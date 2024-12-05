#include "change.h"
#include "../model/vmstate.h"

// #define ctrl(x) ((x) & 0x1f)

ChangeMode::ChangeMode() {
    mode = Mode::NORMAL;
}

void ChangeMode::doAction(const std::vector<int> &input, VMState *vmstate) {
    // vmstate->insert(input);
    if(input.at(0) == 'a') {
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getController()->setMode(Mode::INSERT);
    } else if(input.at(0) == ':') {
        vmstate->getCommandBarState()->setCommandBar(":");
        vmstate->getController()->setMode(Mode::COMMAND);
    }
    vmstate->getController()->flushBuffer();
}

bool ChangeMode::matchAction(const std::vector<int> &input) {
    if(input.size() == 1 && (input.at(0) == 'a' || input.at(0) == ':')) {
        return true;
    }
    return false;
}

ChangeMode::~ChangeMode() = default;
