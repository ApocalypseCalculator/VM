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
                vmstate->getCommandBarState()->setError("File has unsaved changes");
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
                vmstate->getCommandBarState()->setError("File is read-only");
            } else if (vmstate->getFileState()->getFilename() == "") {
                vmstate->getCommandBarState()->setError("No filename specified");
            } else {
                vmstate->getFileState()->save();
                vmstate->getCommandBarState()->setCommandBar("File saved");
                if(input == "wq\n") vmstate->terminate();
            }
        }
        else if(input == "$\n") {
            int linec = vmstate->getFileState()->getLineCount() - 1;
            if(linec < 0) linec = 0;
            vmstate->getFileState()->setCursor(Cursor{linec, 0});
        }
        else if(input == "0\n") {
            vmstate->getFileState()->setCursor(Cursor{0, 0});
        }
        else {
            std::string inp{input.begin(), input.end()};
            inp.pop_back();
            int converted = 0;
            try {
                converted = std::stoi(inp.c_str());
            } catch(...) {}
            if(converted > 0) {
                vmstate->getFileState()->setCursor(Cursor{0, 0});
                vmstate->getFileState()->moveCursor(0, converted-1, true);
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
