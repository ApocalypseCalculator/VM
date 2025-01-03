#ifndef ___CHANGE____H_
#define ___CHANGE____H_

#include <string>
#include <vector>

struct LineChange {
    std::string newline;
    std::string oldline;
    bool newdeleted = false;
    bool olddeleted = false;
    int lineidx;
};

struct Change {
    std::vector<LineChange> changes;
};

#endif
