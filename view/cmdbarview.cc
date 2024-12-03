#include "cmdbarview.h"
#include <ncurses.h>

CommandBarView::CommandBarView() {
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    initview(3, maxx, maxy - 3, 0);
}
void CommandBarView::update(const std::string& msg) {}
void CommandBarView::updateLine(int row, const std::string) {};
void CommandBarView::updateCursor(int row, int col) {};
void CommandBarView::displayView() {};