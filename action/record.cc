#include "record.h"
#include "../model/vmstate.h"

RecordReplay::RecordReplay() {
    mode = Mode::NORMAL;
}

// note q is not repeatable, but @ is
void RecordReplay::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.at(0) == 'q' && input.size() == 1) {
        if(vmstate->getMacros()->isRecording() != '\0') {
            vmstate->getMacros()->stopRecording();
            vmstate->getCommandBarState()->setPersistMessage("");
        }
        else {
            // note: do NOT flush buffer if q is not stopping recording
            return;
        }
    } // the following have input size 2
    else if(input.at(0) == '@') {
        if(input.at(1) == '@') {
            if(vmstate->getMacros()->getLast() != '\0') {
                // this macro is guaranteed to exist, since we don't delete
                vmstate->getMacros()->replay(vmstate->getMacros()->getLast(), vmstate);
                vmstate->getCommandBarState()->setCommandBar("replaying @" + std::string(1, vmstate->getMacros()->getLast()));
            }
            else {
                vmstate->getCommandBarState()->setError("No last macro");
            }
        }
        else if(vmstate->getMacros()->replay(input.at(1), vmstate)) {
            vmstate->getCommandBarState()->setCommandBar("replaying @" + std::string(1, input.at(1)));
        }
        else {
            vmstate->getCommandBarState()->setError("Macro not found");
        }
    }
    else if(input.at(0) == 'q') {
        vmstate->getMacros()->record(input.at(1));
        vmstate->getCommandBarState()->setPersistMessage("recording @" + std::string(1, input.at(1)));
    }
    vmstate->getController()->flushBuffer();
}

bool RecordReplay::matchAction(const std::vector<int> &input) {
    if(input.size() == 2 && (input.at(0) == 'q' || input.at(0) == '@')) {
        return true;
    }
    else if(input.size() == 1 && input.at(0) == 'q') {
        return true;
    }
    return false;
}

RecordReplay::~RecordReplay() = default;
