#ifndef ___COPY_ACTION___H__
#define ___COPY_ACTION___H__

#include "action.h"

class CopyPaste : public Action {
    public:
    CopyPaste();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~CopyPaste() override;
};

#endif