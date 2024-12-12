#ifndef ___CHANGEMODE_ACTION___H__
#define ___CHANGEMODE_ACTION___H__
#include <memory>
#include "action.h"
#include "copy.h"

class ChangeMode : public Action {
    std::unique_ptr<CopyPaste> copypaste;
    public:
    ChangeMode();
    void doAction(const std::vector<int> &input, VMState *vmstate, int multiplier) override;
    bool matchAction(const std::vector<int> &input) override;
    ~ChangeMode() override;
};
#endif
