#ifndef ___MOVECURSOR_ACTION___H__
#define ___MOVECURSOR_ACTION___H__
#include "action.h"

class MoveCursor : public Action {
    public:
    MoveCursor();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~MoveCursor() override;
};
#endif
