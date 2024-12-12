#include <memory>
#include "copy.h"
#include "../model/vmstate.h"
#include "move.h"

CopyPaste::CopyPaste() {
    mode = Mode::NORMAL;
    move = std::make_unique<MoveCursor>();
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

    if(input.at(0) == 'p' || input.at(0) == 'P') {
        if(input.at(0) == 'p') vmstate->getFileState()->moveCursor(1, 0, false);
        for(char c : vmstate->getClipboard()->get()) {
            vmstate->getFileState()->insertChar(c);
        }
        // move cursor back
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
    else if(input.at(0) == 'y') { // YANK (COPY)
        if(input.at(1) == 'y') { // yank line
            vmstate->getClipboard()->set(vmstate->getFileState()->getLine(vmstate->getFileState()->getCursor().lineidx));
        } else {
            Cursor curpos = vmstate->getFileState()->getCursor();
            move->doAction(std::vector<int>(input.begin() + 1, input.end()), vmstate);
            Cursor newpos = vmstate->getFileState()->getCursor();
            // now find the difference between the two cursors, this is the content 
            // we yank/copy
            if(curpos.lineidx == newpos.lineidx) {
                // same line
                if(curpos.charidx < newpos.charidx) {
                    vmstate->getClipboard()->set(vmstate->getFileState()->getLine(curpos.lineidx).substr(curpos.charidx, newpos.charidx - curpos.charidx));
                } else {
                    vmstate->getClipboard()->set(vmstate->getFileState()->getLine(curpos.lineidx).substr(newpos.charidx, curpos.charidx - newpos.charidx));
                }
            } else {
                // different lines
                std::string yank = vmstate->getFileState()->getLine(curpos.lineidx).substr(curpos.charidx);
                yank += "\n";
                for(int i = curpos.lineidx + 1; i < newpos.lineidx; i++) {
                    yank += vmstate->getFileState()->getLine(i);
                    yank += "\n";
                }
                yank += vmstate->getFileState()->getLine(newpos.lineidx).substr(0, newpos.charidx);
                vmstate->getClipboard()->set(yank);
            }
            // reset cursor back to original position
            vmstate->getFileState()->setCursor(curpos);
        }
    }
    else if(input.at(0) == 'd') { // DELETE (CUT)
        if(input.at(1) == 'd') { // delete line
            vmstate->getClipboard()->set(vmstate->getFileState()->getLine(vmstate->getFileState()->getCursor().lineidx));
            vmstate->getFileState()->removeLine(vmstate->getFileState()->getCursor().lineidx);
        } else {
            Cursor curpos = vmstate->getFileState()->getCursor();
            move->doAction(std::vector<int>(input.begin() + 1, input.end()), vmstate);
            Cursor newpos = vmstate->getFileState()->getCursor();
            // now find the difference between the two cursors, this is the content 
            // we delete/cut
            if(curpos.lineidx == newpos.lineidx) {
                // same line
                if(curpos.charidx < newpos.charidx) {
                    vmstate->getClipboard()->set(vmstate->getFileState()->getLine(curpos.lineidx).substr(curpos.charidx, newpos.charidx - curpos.charidx));
                } else {
                    vmstate->getClipboard()->set(vmstate->getFileState()->getLine(curpos.lineidx).substr(newpos.charidx, curpos.charidx - newpos.charidx));
                }
            } else {
                // different lines
                std::string yank = vmstate->getFileState()->getLine(curpos.lineidx).substr(curpos.charidx);
                yank += "\n";
                for(int i = curpos.lineidx + 1; i < newpos.lineidx; i++) {
                    yank += vmstate->getFileState()->getLine(i);
                    yank += "\n";
                }
                yank += vmstate->getFileState()->getLine(newpos.lineidx).substr(0, newpos.charidx);
                vmstate->getClipboard()->set(yank);
            }

            // if we are deleting from bottom to top, we need to adjust the cursor
            if(curpos.lineidx > newpos.lineidx || (curpos.lineidx == newpos.lineidx && curpos.charidx > newpos.charidx)) {
                std::swap(curpos, newpos);
            }
            // now curpos is guaranteed to be before newpos
            if(curpos.lineidx == newpos.lineidx) {
                vmstate->getFileState()->replaceLine(curpos.lineidx, vmstate->getFileState()->getLine(curpos.lineidx).substr(0, curpos.charidx) + vmstate->getFileState()->getLine(curpos.lineidx).substr(newpos.charidx));
            } else {
                vmstate->getFileState()->replaceLine(curpos.lineidx, vmstate->getFileState()->getLine(curpos.lineidx).substr(0, curpos.charidx) + vmstate->getFileState()->getLine(newpos.lineidx).substr(newpos.charidx));
                for(int i = curpos.lineidx + 1; i < newpos.lineidx; i++) {
                    vmstate->getFileState()->removeLine(curpos.lineidx + 1);
                }
                vmstate->getFileState()->removeLine(newpos.lineidx);
            }

            // reset cursor back to front position
            vmstate->getFileState()->setCursor(curpos);
        }
    }
    else if(input.at(0) == 'c') { // CHANGE (DELETE)
        if(input.at(1) == 'c') { // change line
            vmstate->getFileState()->replaceLine(vmstate->getFileState()->getCursor().lineidx, "");
        } else {
            Cursor curpos = vmstate->getFileState()->getCursor();
            move->doAction(std::vector<int>(input.begin() + 1, input.end()), vmstate);
            Cursor newpos = vmstate->getFileState()->getCursor();
            // now find the difference between the two cursors

            // if we are deleting from bottom to top, we need to adjust the cursor
            if(curpos.lineidx > newpos.lineidx || (curpos.lineidx == newpos.lineidx && curpos.charidx > newpos.charidx)) {
                std::swap(curpos, newpos);
            }
            // now curpos is guaranteed to be before newpos
            if(curpos.lineidx == newpos.lineidx) {
                vmstate->getFileState()->replaceLine(curpos.lineidx, vmstate->getFileState()->getLine(curpos.lineidx).substr(0, curpos.charidx) + vmstate->getFileState()->getLine(curpos.lineidx).substr(newpos.charidx));
            } else {
                vmstate->getFileState()->replaceLine(curpos.lineidx, vmstate->getFileState()->getLine(curpos.lineidx).substr(0, curpos.charidx) + vmstate->getFileState()->getLine(newpos.lineidx).substr(newpos.charidx));
                for(int i = curpos.lineidx + 1; i < newpos.lineidx; i++) {
                    vmstate->getFileState()->removeLine(curpos.lineidx + 1);
                }
                vmstate->getFileState()->removeLine(newpos.lineidx);
            }

            // reset cursor back to front position
            vmstate->getFileState()->setCursor(curpos);
        }
        vmstate->getController()->setMode(Mode::INSERT);
        vmstate->getCommandBarState()->setCommandBar("-- INSERT --");
        vmstate->getHistory()->setRecentEditCmd(input);
    }

    // if its not a yank motion, then change has been applied somewhere
    if(input.at(0) != 'y') {
        vmstate->getHistory()->createChange(vmstate);
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
    else if(input.size() > 1) {
        if(input.at(0) == 'y') {
            if(input.size() == 2 && input.at(1) == 'y') {
                return true;
            } else if (move->matchAction(std::vector<int>(input.begin() + 1, input.end()))) {
                return true;
            }
        }
        else if(input.at(0) == 'd') {
            if(input.size() == 2 && input.at(1) == 'd') {
                return true;
            } else if (move->matchAction(std::vector<int>(input.begin() + 1, input.end()))) {
                return true;
            }
        } else if(input.at(0) == 'c') {
            if(input.size() == 2 && input.at(1) == 'c') {
                return true;
            } else if (move->matchAction(std::vector<int>(input.begin() + 1, input.end()))) {
                return true;
            }
        }
    }
    return false;
}

CopyPaste::~CopyPaste() = default;
