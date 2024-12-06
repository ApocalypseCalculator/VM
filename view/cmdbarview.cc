#include "cmdbarview.h"
#include "../model/cmdbarstate.h"
#include "../model/cursor.h"
#include <ncurses.h>

#define ERRORTEXT 9

CommandBarView::CommandBarView(CommandBarState *cmdbar) : cmdbar{cmdbar} {
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    win = newwin(3, maxx, maxy - 3, 0);
    if(has_colors()) {
        init_pair(ERRORTEXT, COLOR_WHITE, COLOR_RED);
        coloured = true;
    }
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
    if(cmdbar->getError().size() == 0) {
        wprintw(win, cmdbar->getCommandBar().c_str());
    } else {
        if(coloured) wattron(win, COLOR_PAIR(ERRORTEXT));
        wprintw(win, cmdbar->getError().c_str());
        if(coloured) wattroff(win, COLOR_PAIR(ERRORTEXT));
    }
    wmove(win, 1, cmdbar->getCursor().charidx + 1);
    wrefresh(win);
}
