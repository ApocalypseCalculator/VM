#include <memory>
#include "model.h"
#include "vmstate.h"
#include "../view/cmdbarview.h"
#include "../view/fileview.h"
#include "../controller/curseskb.h"
#include "vmcmdbarstate.h"
#include "vmfilestate.h"
#include "vmclipboard.h"
#include "vmmacros.h"
#include "vmedithistory.h"
#include <ncurses.h>

VMState::VMState(const std::string& filename) {
    if(filename != "") {
        file = std::make_unique<VMFileState>(filename);
    } else {
        file = std::make_unique<VMFileState>();
    }
    cmdbar = std::make_unique<VMCommandBarState>();
    clipboard = std::make_unique<VMClipboard>();
    macros = std::make_unique<VMMacros>();
    history = std::make_unique<VMEditHistory>(file.get());

    initscr();
    set_escdelay(0);
    if(has_colors()) {
        start_color();
        use_default_colors();
    }
    std::unique_ptr<View> cmdbarview = std::make_unique<CommandBarView>(cmdbar.get());
    std::unique_ptr<View> fileview = std::make_unique<FileView>(file.get());
    addView(std::move(cmdbarview));
    addView(std::move(fileview));
    std::unique_ptr<Controller> control = std::make_unique<CursesKeyboard>(macros.get());
    setController(std::move(control));
}

void VMState::run() {
    Action* res = control->getAction();
    std::vector<int> bufcpy = control->getBuffer();
    if(res != nullptr) {
        if(control->getMultiplier() == 0) {
            if(res->isSelfDefMultiply()) {
                res->doAction(bufcpy, this, 1);
            }
            else {
                res->doAction(bufcpy, this);
            }
        }
        else {
            if(res->isSelfDefMultiply()) {
                res->doAction(bufcpy, this, control->getMultiplier());
            }
            else {
                int multiplier = control->getMultiplier();
                for(int i = 0; i < multiplier; i++) {
                    res->doAction(bufcpy, this);
                }
            }
        }
    }
    cmdbar->setCursor(file->getCursor());
    if(file->getLine(file->getCursor().lineidx).size() == 0) {
        cmdbar->setCursor(Cursor{file->getCursor().lineidx, -1});
    }
    for(auto& view : views) {
        view->displayView();
    }
}

VMState::~VMState() {
    endwin();
}

CommandBarState* VMState::getCommandBarState() {
    return cmdbar.get();
}

FileState* VMState::getFileState() {
    return file.get();
}

Clipboard* VMState::getClipboard() {
    return clipboard.get();
}

Macros* VMState::getMacros() {
    return macros.get();
}

EditHistory* VMState::getHistory() {
    return history.get();
}
