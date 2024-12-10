#include "search.h"
#include "../model/vmstate.h"
#include <string>
#include <string_view>

Search::Search() {
    mode = Mode::NORMAL;
}

// assume input contains only either n or N
void Search::doAction(const std::vector<int> &input, VMState *vmstate) {
    bool searchDirection = vmstate->getCommandBarState()->getSearchSForward();
    std::string searchstr = vmstate->getCommandBarState()->getSearchStr();
    if(searchstr == "") {
        vmstate->getCommandBarState()->setError("No previous search pattern");
        vmstate->getController()->flushBuffer();
        return;
    }
    
    if(input.at(0) == 'N') searchDirection = !searchDirection;
    vmstate->getCommandBarState()->setCommandBar((searchDirection ? "/": "?") + searchstr);
    // search impl
    Cursor starting = vmstate->getFileState()->getCursor();
    Cursor search = starting;
    if(searchDirection) { // fwd search
        bool looped = false; // if we looped around the file
        bool found = false;

        // we start by incrementing search cursor (this is fine since we don't update unless we find)
        if(vmstate->getFileState()->getLine(search.lineidx).size() == search.charidx + 1) {
            search.lineidx++;
            search.charidx = 0;
            if(search.lineidx == vmstate->getFileState()->getLineCount()) {
                search.lineidx = 0;
                looped = true;
            }
        } else {
            search.charidx++;
        }

        while(!looped || (search.lineidx < starting.lineidx || search.charidx < starting.charidx)) {
            std::string line = vmstate->getFileState()->getLine(search.lineidx);
            size_t foundidx = line.find(searchstr, search.charidx);
            if(foundidx == std::string::npos) {
                search.lineidx++;
                search.charidx = 0;
                if(search.lineidx == vmstate->getFileState()->getLineCount()) {
                    search.lineidx = 0;
                    looped = true;
                }
            }
            else {
                found = true;
                search.charidx = foundidx;
                break;
            }
        }
        if(!found) {
            vmstate->getCommandBarState()->setError("Pattern not found: " + searchstr);
        } else {
            if(looped) {
                vmstate->getCommandBarState()->setError("search hit BOTTOM, continuing at TOP");
            }
            vmstate->getFileState()->setCursor(search);
        }
    }
    else { // bwd search
        bool looped = false; // if we looped around the file
        bool found = false;

        if(search.charidx == 0) {
            search.lineidx--;
            if(search.lineidx < 0) {
                search.lineidx = vmstate->getFileState()->getLineCount() - 1;
                looped = true;
            }
            search.charidx = vmstate->getFileState()->getLine(search.lineidx).size()-1;
            if(search.charidx < 0) search.charidx = 0;
        } else {
            search.charidx--;
        }

        while(!looped || (search.lineidx > starting.lineidx || search.charidx > starting.charidx)) {
            std::string line = vmstate->getFileState()->getLine(search.lineidx);
            std::string_view linefrange{line.begin(), line.begin() + search.charidx};
            size_t foundidx = linefrange.rfind(searchstr);
            if(foundidx == std::string::npos) {
                search.lineidx--;
                if(search.lineidx < 0) {
                    search.lineidx = vmstate->getFileState()->getLineCount() - 1;
                    looped = true;
                }
                search.charidx = vmstate->getFileState()->getLine(search.lineidx).size()-1;
                if(search.charidx < 0) search.charidx = 0;
            }
            else {
                found = true;
                search.charidx = foundidx;
                break;
            }
        }

        if(!found) {
            vmstate->getCommandBarState()->setError("Pattern not found: " + searchstr);
        } else {
            if(looped) {
                vmstate->getCommandBarState()->setError("search hit TOP, continuing at BOTTOM");
            }
            vmstate->getFileState()->setCursor(search);
        }
    }

    vmstate->getController()->flushBuffer();
}

bool Search::matchAction(const std::vector<int> &input) {
    if(input.size() == 1 && (input.at(0) == 'n' || input.at(0) == 'N')) {
        return true;
    }
    return false;
}

Search::~Search() = default;
