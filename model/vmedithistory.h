#ifndef ___VM_EDIT_HISTORY____H_
#define ___VM_EDIT_HISTORY____H_

#include "edithistory.h"
#include <stack>

class FileState;

class VMEditHistory : public EditHistory {
    std::stack<Change> changes;
    // content of the file at the time of the last change
    std::vector<std::string> cachecopy;
    std::vector<int> recentEditCmd;
    public: 
    VMEditHistory(FileState* fstate);
    void undo(VMState* vmstate) override;
    void addChange(Change change) override;
    void createChange(VMState* fstate) override;
    void redo(VMState* vmstate) override;
    void setRecentEditCmd(std::vector<int> cmd) override;
    bool isEmpty() override;
};

#endif
