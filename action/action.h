#ifndef ___ACTION___H__
#define ___ACTION___H__

#include <vector>

class VMState;

enum Mode { NORMAL, COMMAND, INSERT, REPLACE };

class Action {
    protected:
    bool selfdefmultiply = false;
    bool redoable = false;
    Mode mode;
    public: 
    virtual void doAction(const std::vector<int> &input, VMState *vmstate) {};
    virtual void doAction(const std::vector<int> &input, VMState *vmstate, int multiplier) {};
    virtual bool matchAction(const std::vector<int> &input) = 0;
    virtual ~Action() = default;
    const bool isSelfDefMultiply() const { return selfdefmultiply; }
    const bool isRedoable() const { return redoable; }
    const Mode getMode() const { return mode; }
};

#endif
