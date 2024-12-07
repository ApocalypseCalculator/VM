#ifndef ___REPLACE_ACTION___H__
#define ___REPLACE_ACTION___H__
#include "action.h"

class Replace : public Action {
    public:
    Replace();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~Replace() override;
};
#endif
