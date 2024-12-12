#ifndef ___EDIT_HISTORY____H_
#define ___EDIT_HISTORY____H_

#include "vmstate.h"
#include "change.h"

class EditHistory {
    public: 
    // undo most recent change
    virtual void undo(VMState* vmstate) = 0;
    // add a change to the history
    virtual void addChange(Change change) = 0;
    // diff the file with cache and create a change
    virtual void createChange(VMState* fstate) = 0;
    // redo the last change (creates a new change)
    virtual void redo(VMState* vmstate) = 0;
    // set the most recent edit command (directly use cmd char)
    virtual void setRecentEditCmd(std::vector<int> cmd) = 0;
    virtual bool isEmpty() = 0;
};

#endif
