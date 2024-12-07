#ifndef ___MOVECURSOR_ACTION___H__
#define ___MOVECURSOR_ACTION___H__
#include "action.h"
#include "../model/cursor.h"
#include "../model/filestate.h"

class MoveCursor : public Action {
    // search for matching character in file
    Cursor findMatch(Cursor start, FileState *file);
    public:
    MoveCursor();
    void doAction(const std::vector<int> &input, VMState *vmstate) override;
    bool matchAction(const std::vector<int> &input) override;
    ~MoveCursor() override;
};
#endif
