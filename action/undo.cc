#include "undo.h"

UndoRedo::UndoRedo() = default;

UndoRedo::~UndoRedo() = default;

void UndoRedo::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input[0] == 'u') {
        if(vmstate->getHistory()->isEmpty()) {
            vmstate->getCommandBarState()->setError("Already at oldest change");
        }
        else {
            vmstate->getHistory()->undo(vmstate);
        }
    }
    else if(input[0] == '.') {
        vmstate->getHistory()->redo(vmstate);
    }
    vmstate->getController()->flushBuffer();
}

bool UndoRedo::matchAction(const std::vector<int> &input) {
    return input.size() == 1 && (input[0] == 'u' || input[0] == '.');
}
