#include "fileview.h"
#include "../model/filestate.h"
#include <ncurses.h>

#define EMPTYROW 1

FileView::FileView(FileState *file): file{file} {
    width = getmaxx(stdscr);
    height = getmaxy(stdscr) - 3;
    win = newwin(height, width, 0, 0);
    if(has_colors()) {
        start_color();
        init_pair(EMPTYROW, COLOR_MAGENTA, COLOR_BLACK);
        coloured = true;
    }
    refresh();
    displayView();
}
void FileView::update(const std::string& msg) {}
void FileView::updateLine(int row, const std::string) {};
void FileView::updateCursor(int row, int col) {};
void FileView::displayView() {
    wclear(win);
    box(win, 0, 0);
    for(int i = 0; i < height-2; i++) { // -2 to leave space for border box
        if(i < file->getLineCount()) {
            int pos = 1;
            for(auto c : file->getLine(i)) {
                wmove(win, i + 1, pos);
                waddch(win, c);
                pos++;
            }
            // wprintw(win, file->getLine(i).c_str());
        } else {
            wmove(win, i + 1, 1);
            if(coloured) wattron(win, COLOR_PAIR(EMPTYROW));
            waddch(win, '~');
            if(coloured) wattroff(win, COLOR_PAIR(EMPTYROW));
        }
    }
    wmove(win, 1 + file->getCursor().lineidx, 1 + file->getCursor().charidx);
    wrefresh(win);
};
