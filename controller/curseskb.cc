#include <vector>
#include "curseskb.h"
#include "../action/action.h"
#include <ncurses.h>

CursesKeyboard::CursesKeyboard() {
    cbreak();
    noecho();
    keypad(stdscr, true);
}

Action* CursesKeyboard::getAction() {
    int ch = getch();
    buffer.push_back(ch);
    return parser.parseAction(buffer);
}

CursesKeyboard::~CursesKeyboard() {}
