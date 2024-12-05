#include "command.h"
#include "../model/vmstate.h"
#include <ncurses.h>

Command::Command() {
    mode = Mode::COMMAND;
}

bool operator==(const std::vector<int> &input, std::string s) {
    if(input.size() != s.size()) {
        return false;
    }
    for(size_t i = 0; i < s.size(); i++) {
        if(input.at(i) != s.at(i)) {
            return false;
        }
    }
    return true;
}

void Command::doAction(const std::vector<int> &input, VMState *vmstate) {
    // vmstate->insert(input);
    vmstate->getCommandBarState()->appendCommandBar(input.back());
    if(input == "q!\n") {
        vmstate->terminate();
    }
    // vmstate->getController()->flushBuffer();
}

bool Command::matchAction(const std::vector<int> &input) {
    return true;
}

Command::~Command() = default;
