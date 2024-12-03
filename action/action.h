#ifndef ___ACTION___H__
#define ___ACTION___H__

#include <vector>

class VMState;

enum Mode { NORMAL, COMMAND, INSERT, REPLACE };

class Action {
    public: 
    Mode mode;
    virtual void doAction(const std::vector<int> &input, VMState *vmstate) = 0;
    virtual bool matchAction(const std::vector<int> &input) = 0;
    virtual ~Action();
};

#endif
