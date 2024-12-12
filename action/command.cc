#include "command.h"
#include "../model/vmstate.h"
#include <ncurses.h>
#include <vector>
#include <fstream>

Command::Command() {
    mode = Mode::COMMAND;
    selfdefmultiply = true;
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

void Command::doAction(const std::vector<int> &unfiltered, VMState *vmstate, int multiplier) {
    std::vector<int> input = unfiltered;
    int toremove = 0;
    for(int i = input.size()-1; i >= 0; i--) {
        if(input.at(i) == 127 || input.at(i) == KEY_BACKSPACE || input.at(i) == KEY_DC) { //backspace key
            input.erase(input.begin() + i);
            toremove++;
        }
        else if(toremove > 0) {
            input.erase(input.begin() + i);
            toremove--;
        }
    }

    if(unfiltered.empty()) return;
    // vmstate->insert(input);
    if(input.empty()) { // command has been aborted by backspace
        vmstate->getController()->flushBuffer();
        vmstate->getCommandBarState()->setCommandBar("");
        vmstate->getController()->setMode(Mode::NORMAL);
        return;
    }
    if(unfiltered.back() == 127 || unfiltered.back() == KEY_BACKSPACE || unfiltered.back() == KEY_DC) { // show backspace visually
        vmstate->getCommandBarState()->removeChar();
        return;
    }

    vmstate->getCommandBarState()->appendCommandBar(unfiltered.back());
    // ESC key pressed, abort command
    if(input.back() == 27) {
        vmstate->getController()->flushBuffer();
        vmstate->getCommandBarState()->setCommandBar("");
        vmstate->getController()->setMode(Mode::NORMAL);
    }
    else if(input.back() == '\n') { // command entered
        if(input.at(0) == ':') { // : commands
        if(input == ":q\n") {
            if(vmstate->getFileState()->hasChange()) {
                vmstate->getCommandBarState()->setError("File has unsaved changes");
            } else {
                vmstate->terminate();
            }
        }
        else if(input == ":q!\n") {
            vmstate->terminate();
        }
        else if(input == ":w\n" || input == ":wq\n") {
            // just :w\n or :wq\n
            if(vmstate->getFileState()->isReadOnly()) {
                vmstate->getCommandBarState()->setError("File is read-only");
            } else if (vmstate->getFileState()->getFilename() == "") {
                vmstate->getCommandBarState()->setError("No filename specified");
            } else {
                vmstate->getFileState()->save();
                vmstate->getCommandBarState()->setCommandBar("File saved");
                if(input == ":wq\n") vmstate->terminate();
            }
        }
        else if(input == ":$\n") {
            int linec = vmstate->getFileState()->getLineCount() - 1;
            if(linec < 0) linec = 0;
            vmstate->getFileState()->setCursor(Cursor{linec, 0});
        }
        else if(input == ":0\n") {
            vmstate->getFileState()->setCursor(Cursor{0, 0});
        }
        else if(input.size() > 4 && input.at(1) == 'w' && input.at(2) == ' ') { // :w filename\n
            std::string filename{input.begin() + 3, input.end()-1};
            vmstate->getFileState()->save(filename);
            vmstate->getCommandBarState()->setCommandBar("File saved");
        }
        else if(input.size() > 5 && input.at(1) == 'w' && input.at(2) == 'q' && input.at(3) == ' ') { // :w filename\n
            std::string filename{input.begin() + 4, input.end()-1};
            vmstate->getFileState()->save(filename);
            vmstate->getCommandBarState()->setCommandBar("File saved");
            vmstate->terminate();
        }
        else if(input.size() > 2 && input.at(1) == 'r') {
            if(input.size() == 3) { // then its :r\n
                if (vmstate->getFileState()->getFilename() == "") {
                    vmstate->getCommandBarState()->setError("No filename specified");
                }
                else {
                    std::ifstream filetoread = std::ifstream{vmstate->getFileState()->getFilename()};
                    if(filetoread.fail()) {
                        vmstate->getCommandBarState()->setError("Error reading file");
                    }
                    else {
                        std::string lineval;
                        while(getline(filetoread, lineval)) {
                            vmstate->getFileState()->insert(lineval);
                        }
                        filetoread.close();
                    }
                }
            } else if(input.size() > 4 && input.at(2) == ' ') { // supplied filename as arg
                std::string filename{input.begin() + 3, input.end()-1};
                std::ifstream filetoread = std::ifstream{filename};

                if(filetoread.fail()) {
                    vmstate->getCommandBarState()->setError("Error reading file");
                }
                else {
                    std::string lineval;
                    while(getline(filetoread, lineval)) {
                        vmstate->getFileState()->insert(lineval);
                    }
                    filetoread.close();
                }
            }
        }
        else {
            std::string inp{input.begin(), input.end()};
            inp = inp.substr(1, inp.size() - 2);
            int converted = 0;
            try {
                converted = std::stoi(inp.c_str());
            } catch(...) {}
            if(converted > 0) {
                vmstate->getFileState()->setCursor(Cursor{0, 0});
                vmstate->getFileState()->moveCursor(0, converted-1, true);
            }
        }
        }
        else { // / or ?
            if(input.at(0) == '/') {
                vmstate->getCommandBarState()->setSearchSForward(true);
            } else if(input.at(0) == '?') {
                vmstate->getCommandBarState()->setSearchSForward(false);
            }
            std::string search{input.begin() + 1, input.end()-1};
            if(search.size() == 0) {
                // get last search
                search = vmstate->getCommandBarState()->getSearchStr();
                if(search == "") {
                    vmstate->getCommandBarState()->setError("No previous search");
                }
            } else {
                vmstate->getCommandBarState()->setSearch(search);
            }

            if(search != "") {
                // call search action (in same direction as defined)
                std::string multed = "n";
                if(multiplier > 1) {
                    multed = std::to_string(multiplier) + multed;
                }
                vmstate->getController()->setReplay(multed);
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
