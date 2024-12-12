#ifndef ___COPY_ACTION___H__
#define ___COPY_ACTION___H__

#include "action.h"
#include "move.h"

class CopyPaste : public Action {
    // move cursor action
    std::unique_ptr<MoveCursor> move;
    public:
    CopyPaste();
    void doAction(const std::vector<int> &input, VMState *vmstate, int multiplier) override;
    bool matchAction(const std::vector<int> &input) override;
    ~CopyPaste() override;
};

#endif