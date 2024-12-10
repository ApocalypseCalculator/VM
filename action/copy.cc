#include "copy.h"
#include "../model/vmstate.h"

CopyPaste::CopyPaste() {
    mode = Mode::NORMAL;
}

// valid single char inputs
static const int validInputs[] = {
    'p',
    'P',
    'x',
    'X'
};

void CopyPaste::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(vmstate->getClipboard()->get() == "" && (input.at(0) == 'p' || input.at(0) == 'P')) {
        vmstate->getCommandBarState()->setError("Clipboard is empty");
        vmstate->getController()->flushBuffer();
        return;
    }

    if(input.at(0) == 'p') {
        vmstate->getFileState()->moveCursor(1, 0, false);
        for(char c : vmstate->getClipboard()->get()) {
            vmstate->getFileState()->insertChar(c);
        }
        // move cursor back by 1
        vmstate->getFileState()->moveCursor(-1, 0, false);
    } else if(input.at(0) == 'P') {
        for(char c : vmstate->getClipboard()->get()) {
            vmstate->getFileState()->insertChar(c);
        }
        vmstate->getFileState()->moveCursor(-1, 0, false);
    } else if(input.at(0) == 'x') {
        if(vmstate->getFileState()->getLine(vmstate->getFileState()->getCursor().lineidx).size() != 0) {
            char c = vmstate->getFileState()->getLine(vmstate->getFileState()->getCursor().lineidx).at(vmstate->getFileState()->getCursor().charidx);
            vmstate->getFileState()->moveCursor(1, 0, false);
            vmstate->getFileState()->removeChar();
            vmstate->getClipboard()->set(std::string(1, c));
        }
    } else if(input.at(0) == 'X') {
        // if 0, do nothing
        if(vmstate->getFileState()->getCursor().charidx != 0) {
            // get char before cursor
            char c = vmstate->getFileState()->getLine(vmstate->getFileState()->getCursor().lineidx).at(vmstate->getFileState()->getCursor().charidx-1);
            vmstate->getFileState()->removeChar();
            vmstate->getClipboard()->set(std::string(1, c));
        }
    }

    vmstate->getController()->flushBuffer();
}

bool CopyPaste::matchAction(const std::vector<int> &input) {
    if(input.size() == 1) {
        for(auto &i : validInputs) {
            if(input.at(0) == i) {
                return true;
            }
        }
    }
    return false;
}

CopyPaste::~CopyPaste() = default;
