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
        if(vmstate->getFileState()->isChanged()) {
            changed = " [Modified] ";
        }

        vmstate->getCommandBarState()->setCommandBar(fname + changed + lcountmsg + pctmsg);
    }
    vmstate->getController()->flushBuffer();
}

bool Normal::matchAction(const std::vector<int> &input) {
    return input.size() == 1 && input.at(0) == ctrl('g');
}

Normal::~Normal() = default;
