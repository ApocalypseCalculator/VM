#ifndef ___UNDO_ACTION___H__
#define ___UNDO_ACTION___H__

#include "action.h"
#include "../model/vmstate.h"

class UndoRedo : public Action {
    public:
    UndoRedo();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~UndoRedo() override;
};

#endif