#include "cmdbarview.h"
#include "../model/cmdbarstate.h"
#include "../model/cursor.h"
#include <ncurses.h>

CommandBarView::CommandBarView(CommandBarState *cmdbar) : cmdbar{cmdbar} {
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    win = newwin(3, maxx, maxy - 3, 0);
    refresh();
    displayView();
}
void CommandBarView::update(const std::string& msg) {}
void CommandBarView::updateLine(int row, const std::string) {};
void CommandBarView::updateCursor(int row, int col) {};
void CommandBarView::displayView() {
    wclear(win);
    box(win, 0, 0);
    wmove(win, 1, 1);
    wprintw(win, cmdbar->getCommandBar().c_str());
    wmove(win, 1, cmdbar->getCursor().charidx + 1);
    wrefresh(win);
}
