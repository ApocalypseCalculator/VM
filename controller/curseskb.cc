#include <vector>
#include "curseskb.h"
#include "../action/action.h"
#include <ncurses.h>

CursesKeyboard::CursesKeyboard() {
    cbreak();
    noecho();
    keypad(stdscr, true);
    mode = Mode::NORMAL;
}

Action* CursesKeyboard::getAction() {
    int ch = getch();
    buffer.push_back(ch);
    return parser.parseAction(buffer, mode);
}

CursesKeyboard::~CursesKeyboard() {}

Mode CursesKeyboard::getMode() {
    return mode;
}
void CursesKeyboard::setMode(Mode mode) {
    this->mode = mode;
}
