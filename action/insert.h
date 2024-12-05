#ifndef ___INSERTION_ACTION___H__
#define ___INSERTION_ACTION___H__
#include "action.h"

class Insertion : public Action {
    public:
    Insertion();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~Insertion() override;
};
#endif
