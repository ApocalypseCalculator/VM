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
    if(input.empty()) return;
    // vmstate->insert(input);
    vmstate->getCommandBarState()->appendCommandBar(input.back());
    // ESC key pressed twice consecutively, abort command
    if(input.size() >= 2 && input.at(input.size()-2) == 27 && input.back() == 27) {
        vmstate->getController()->flushBuffer();
        vmstate->getCommandBarState()->setCommandBar("");
        vmstate->getController()->setMode(Mode::NORMAL);
    }
    else if(input.back() == '\n') {
        if(input == "q\n") {
            if(vmstate->getFileState()->hasChange()) {
                vmstate->getCommandBarState()->setCommandBar("File has unsaved changes");
            } else {
                vmstate->terminate();
            }
        }
        else if(input == "q!\n") {
            vmstate->terminate();
        }
        else if(input == "w\n" || input == "wq\n") {
            // just :w\n or :wq\n
            if(vmstate->getFileState()->isReadOnly()) {
                vmstate->getCommandBarState()->setCommandBar("File is read-only");
            } else if (vmstate->getFileState()->getFilename() == "") {
                vmstate->getCommandBarState()->setCommandBar("No filename specified");
            } else {
                vmstate->getFileState()->save();
                vmstate->getCommandBarState()->setCommandBar("File saved");
                if(input == "wq\n") vmstate->terminate();
            }
        }
        vmstate->getController()->flushBuffer();
        vmstate->getController()->setMode(Mode::NORMAL);
    }
    // vmstate->getController()->flushBuffer();
}

bool Command::matchAction(const std::vector<int> &input) {
    return true;
}

Command::~Command() = default;
