#ifndef ___COMMAND_ACTION___H__
#define ___COMMAND_ACTION___H__
#include "action.h"

class Command : public Action {
    public:
    Command();
    void doAction(const std::vector<int> &input, VMState *vmstate, int multiplier) override;
    bool matchAction(const std::vector<int> &input) override;
    ~Command() override;
};
#endif
