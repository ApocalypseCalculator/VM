#ifndef ___NORMAL_ACTION___H__
#define ___NORMAL_ACTION___H__
#include "action.h"

class Normal : public Action {
    public:
    Normal();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~Normal() override;
};
#endif
