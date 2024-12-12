#include "normal.h"
#include "../model/vmstate.h"

#define ctrl(x) ((x) & 0x1f)

// miscellanous actions

Normal::Normal() {
    mode = NORMAL;
}

void Normal::doAction(const std::vector<int> &input, VMState *vmstate) {
    if(input.at(0) == ctrl('g')) {
        std::string fname = vmstate->getFileState()->getFilename();
        if(fname == "") fname = "\"[No Name]\"";

        int lcount = vmstate->getFileState()->getLineCount();
        if(lcount == 1 && vmstate->getFileState()->getLine(0) == "") lcount = 0;
        std::string lcountmsg;
        std::string pctmsg = "";
        if(lcount == 0) {
            lcountmsg = "--No lines in buffer--";
        } else {
            lcountmsg = std::to_string(lcount) + " line";
            if(lcount > 1) lcountmsg += "s";

            // count percentage of file
            Cursor curpos = vmstate->getFileState()->getCursor();
            // formula taken from vim help
            int pct = ((curpos.lineidx+2) * 100 - 99)/lcount;
            if(pct > 100) pct = 100;
            if(pct < 0) pct = 0;
            pctmsg = " --" + std::to_string(pct) + "%%--";
        }

        std::string changed = " ";
        if(vmstate->getFileState()->hasChange()) {
            changed = " [Modified] ";
        }

        vmstate->getCommandBarState()->setCommandBar(fname + changed + lcountmsg + pctmsg);
    } else if (input.at(0) == 'J') {
        // join lines
        Cursor search = vmstate->getFileState()->getCursor();
        if(search.lineidx < vmstate->getFileState()->getLineCount()-1) {
            std::string line = vmstate->getFileState()->getLine(search.lineidx);
            std::string nextline = vmstate->getFileState()->getLine(search.lineidx+1);
            // remove leading whitespace
            while(nextline.size() > 0 && (nextline.at(0) == ' ' || nextline.at(0) == '\t')) {
                nextline = nextline.substr(1);
            }
            vmstate->getFileState()->replaceLine(search.lineidx, line + " " + nextline);
            vmstate->getFileState()->removeLine(search.lineidx+1);
            vmstate->getHistory()->createChange(vmstate);
        }
    }
    vmstate->getController()->flushBuffer();
}

bool Normal::matchAction(const std::vector<int> &input) {
    return input.size() == 1 && (input.at(0) == ctrl('g') || input.at(0) == 'J');
}

Normal::~Normal() = default;
