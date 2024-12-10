#ifndef ___SEARCH_ACTION___H__
#define ___SEARCH_ACTION___H__

#include "action.h"

class Search : public Action {
    public:
    Search();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~Search() override;
};

#endif