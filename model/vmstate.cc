#include "model.h"
#include "vmstate.h"
#include "iostream"
#include "fstream"
#include "../view/cmdbarview.h"
#include "../view/fileview.h"
#include "../controller/curseskb.h"
#include <ncurses.h>

VMState::VMState() {
    //clipboard = new Clipboard();
    //history = new EditHistory();
    //file = new FileState();
    //cmdbar = new CommandBarState();
    //macros = new Macros();
    initscr();
    std::unique_ptr<View> cmdbarview = std::make_unique<CommandBarView>();
    std::unique_ptr<View> fileview = std::make_unique<FileView>();
    addView(std::move(cmdbarview));
    addView(std::move(fileview));
    std::unique_ptr<Controller> control = std::make_unique<CursesKeyboard>();
    setController(std::move(control));
}

void VMState::run() {
    Action* res = control->getAction();
    // std::cout << "Action: " << res << std::endl;
    if(res != nullptr) {
        res->doAction(control->getBuffer(), this);
    }
}

VMState::~VMState() {
    //delete clipboard;
    //delete history;
    //delete file;
    //delete cmdbar;
    //delete macros;
    endwin();
}
