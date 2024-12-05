#include "fileview.h"
#include "../model/filestate.h"
#include <ncurses.h>

FileView::FileView(FileState *file): file{file} {
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    win = newwin(maxy - 3, maxx, 0, 0);
    refresh();
    displayView();
}
void FileView::update(const std::string& msg) {}
void FileView::updateLine(int row, const std::string) {};
void FileView::updateCursor(int row, int col) {};
void FileView::displayView() {
    wclear(win);
    box(win, 0, 0);
    // wmove(win, 1, 1);
    // wprintw(win, cmdbar->getCommandBar().c_str());
    // wmove(win, 1, cmdbar->getCursor().charidx + 1);
    wrefresh(win);
};
