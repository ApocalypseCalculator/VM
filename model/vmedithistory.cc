#include "vmedithistory.h"
#include "filestate.h"
#include "../action/action.h"

VMEditHistory::VMEditHistory(FileState* fstate) {
    cachecopy = std::vector<std::string>();
    for(int i = 0; i < fstate->getLineCount(); i++) {
        cachecopy.push_back(fstate->getLine(i));
    }
}

void VMEditHistory::undo(VMState* vmstate) {
    if(changes.empty()) {
        return;
    }
    Change last = changes.top();
    for(int i = last.changes.size()-1; i >= 0; i--) {
        LineChange linechange = last.changes[i];
        if(linechange.newdeleted) {
            vmstate->getFileState()->insertLine(linechange.lineidx, linechange.oldline);
            if(linechange.lineidx > cachecopy.size()) {
                for(int i = 0; i < linechange.lineidx - cachecopy.size(); i++) {
                    cachecopy.push_back("");
                }
            }
            cachecopy.insert(cachecopy.begin() + linechange.lineidx, linechange.oldline);
        }
        else if(linechange.olddeleted) {
            vmstate->getFileState()->removeLine(linechange.lineidx);
            cachecopy.erase(cachecopy.begin() + linechange.lineidx);
        }
        else {
            vmstate->getFileState()->replaceLine(linechange.lineidx, linechange.oldline);
            cachecopy[linechange.lineidx] = linechange.oldline;
        }
        vmstate->getFileState()->setCursor(Cursor(linechange.lineidx, 0));
    }
    changes.pop();
    if(changes.empty()) {
        vmstate->getFileState()->setChanged(false);
    }
}

void VMEditHistory::addChange(Change change) {
    changes.push(change);
}

void VMEditHistory::createChange(VMState* fstate) {
    Change change;
    if(cachecopy.size() == fstate->getFileState()->getLineCount()) {
        for(int i = 0; i < fstate->getFileState()->getLineCount(); i++) {
            if(cachecopy[i] != fstate->getFileState()->getLine(i)) {
                LineChange linechange;
                linechange.lineidx = i;
                linechange.oldline = cachecopy[i];
                linechange.newline = fstate->getFileState()->getLine(i);
                // update cache
                cachecopy[i] = linechange.newline;
                change.changes.push_back(linechange);
            }
        }
    }
    else if(cachecopy.size() < fstate->getFileState()->getLineCount()) {
        for(int i = 0; i < cachecopy.size(); i++) {
            if(cachecopy[i] != fstate->getFileState()->getLine(i)) {
                LineChange linechange;
                linechange.lineidx = i;
                linechange.oldline = cachecopy[i];
                linechange.newline = fstate->getFileState()->getLine(i);
                // update cache
                cachecopy[i] = linechange.newline;
                change.changes.push_back(linechange);
            }
        }
        for(int i = cachecopy.size(); i < fstate->getFileState()->getLineCount(); i++) {
            LineChange linechange;
            linechange.lineidx = i;
            linechange.oldline = ""; // special case for line that doesnt exist
            linechange.olddeleted = true;
            linechange.newline = fstate->getFileState()->getLine(i);
            // update cache
            cachecopy.push_back(linechange.newline);
            change.changes.push_back(linechange);
        }
    }
    else {
        for(int i = 0; i < fstate->getFileState()->getLineCount(); i++) {
            if(cachecopy[i] != fstate->getFileState()->getLine(i)) {
                LineChange linechange;
                linechange.lineidx = i;
                linechange.oldline = cachecopy[i];
                linechange.newline = fstate->getFileState()->getLine(i);
                // update cache
                cachecopy[i] = linechange.newline;
                change.changes.push_back(linechange);
            }
        }
        for(int i = fstate->getFileState()->getLineCount(); i < cachecopy.size(); i++) {
            LineChange linechange;
            linechange.lineidx = i;
            linechange.oldline = cachecopy[i];
            linechange.newline = ""; // special case for line that doesnt exist
            linechange.newdeleted = true;
            // update cache
            cachecopy[i] = linechange.newline;
            change.changes.push_back(linechange);
        }
    }
    // content = newcontent;
    addChange(change);
}

void VMEditHistory::redo(VMState* vmstate) {
    // redo change
    std::vector<int> buf = vmstate->getController()->getModeBuffer(Mode::INSERT);
    if(buf.empty()) {
        return;
    }
    std::vector<int> result = recentEditCmd;
    // result = recentEditCmd + buf
    result.insert(result.end(), buf.begin(), buf.end());

    vmstate->getController()->setReplay(result);
}

bool VMEditHistory::isEmpty() {
    return changes.empty();
}

void VMEditHistory::setRecentEditCmd(std::vector<int> cmd) {
    recentEditCmd = cmd;
}
