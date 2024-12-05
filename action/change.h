#ifndef ___CHANGEMODE_ACTION___H__
#define ___CHANGEMODE_ACTION___H__
#include "action.h"

class ChangeMode : public Action {
    public:
    ChangeMode();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~ChangeMode() override;
};
#endif
