#include <vector>
#include <ranges>
#include "curseskb.h"
#include "../action/action.h"
#include <ncurses.h>

CursesKeyboard::CursesKeyboard(Macros* macro) : macro{macro} {
    // cbreak();
    raw();
    noecho();
    keypad(stdscr, true);
    mode = Mode::NORMAL;
}

Action* CursesKeyboard::getAction() {
    int ch;
    if(replay.empty()) {
        ch = getch();
    }
    else {
        ch = replay.top();
        replay.pop();
    }
    buffer.push_back(ch);
    if(macro->isRecording() != '\0') {
        // macro needs to remember to delete trailing 'q' key
        macro->append(macro->isRecording(), ch);
    }
    if(ch == 27) { // double escape key press
        if(buffer.size() >= 2 && buffer.at(buffer.size()-2) == 27) {
            buffer.clear();
            mode = Mode::NORMAL;
            return nullptr;
        }
    }
    return parser.parseAction(buffer, mode);
}

CursesKeyboard::~CursesKeyboard() {}

Mode CursesKeyboard::getMode() {
    return mode;
}
void CursesKeyboard::setMode(Mode mode) {
    this->mode = mode;
}

void CursesKeyboard::setReplay(std::string replay) {
    std::ranges::reverse_view reversed {replay};
    for(char c : reversed) {
        this->replay.push(c);
    }
}

void CursesKeyboard::setReplay(std::vector<int> replay) {
    std::ranges::reverse_view reversed {replay};
    for(int c : reversed) {
        this->replay.push(c);
    }
}
