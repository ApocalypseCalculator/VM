#include "fileview.h"
#include <ncurses.h>

FileView::FileView() {
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    initview(maxy - 3, maxx, 0, 0);
}
void FileView::update(const std::string& msg) {}
void FileView::updateLine(int row, const std::string) {};
void FileView::updateCursor(int row, int col) {};
void FileView::displayView() {};