#include "view.h"
#include <ncurses.h>

void View::initview(int h, int w, int y, int x) {
    win = newwin(h, w, y, x);
    box(win, 0, 0);
    refresh();
    wmove(win, 1, 1);
    wrefresh(win);
}
