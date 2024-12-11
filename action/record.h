#ifndef ___RECORD_ACTION___H__
#define ___RECORD_ACTION___H__

#include "action.h"
#include "../model/vmstate.h"

class RecordReplay : public Action {
    public:
    RecordReplay();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~RecordReplay() override;
};

#endif