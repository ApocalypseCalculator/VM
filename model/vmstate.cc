#include <memory>
#include "model.h"
#include "vmstate.h"
#include "../view/cmdbarview.h"
#include "../view/fileview.h"
#include "../controller/curseskb.h"
#include "vmcmdbarstate.h"
#include "vmfilestate.h"
#include <ncurses.h>

VMState::VMState(const std::string& filename) {
    //clipboard = new Clipboard();
    //history = new EditHistory();
    //file = new FileState();
    if(filename != "") {
        file = std::make_unique<VMFileState>(filename);
    } else {
        file = std::make_unique<VMFileState>();
    }
    cmdbar = std::make_unique<VMCommandBarState>();
    //macros = new Macros();
    initscr();
    std::unique_ptr<View> cmdbarview = std::make_unique<CommandBarView>(cmdbar.get());
    std::unique_ptr<View> fileview = std::make_unique<FileView>(file.get());
    addView(std::move(cmdbarview));
    addView(std::move(fileview));
    std::unique_ptr<Controller> control = std::make_unique<CursesKeyboard>();
    setController(std::move(control));
}

void VMState::run() {
    Action* res = control->getAction();
    if(res != nullptr) {
        res->doAction(control->getBuffer(), this);
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
