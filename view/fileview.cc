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

int FileView::getActualDisplayLine(int lineidx, int charidx) {
    int lcount = 0;
    for(int i = 0; i <= lineidx; i++) {
        if(i == lineidx) {
            lcount += charidx / (width - 2);
            break;
        }
        int lineSize = file->getLine(i).size();
        lcount += lineSize / (width - 2);
        if(lineSize == 0) lcount++;
        if(lineSize % (width - 2) != 0) {
            lcount++;
        }
    }
    return lcount;
}

void FileView::updateWindow() {
    Cursor curposition = file->getCursor();

    int curWinStart = getActualDisplayLine(curOffset, 0);
    int curWinEnd = curWinStart + height - 2;
    int curCursorLine = getActualDisplayLine(curposition.lineidx, curposition.charidx);

    if(curCursorLine < curWinStart) {
        curOffset = curposition.lineidx;
    } else if(curCursorLine >= curWinEnd) {
        curOffset++;
    }
}

void FileView::displayView() {
    wclear(win);
    box(win, 0, 0);

    updateWindow();

    int lineidx = curOffset;
    
    Cursor curposition = file->getCursor();

    // cursor position in display
    int dcursCol = 1;
    int dcursRow = 1;

    for(int i = 0; i < height-2; i++) { // -2 to leave space for border box
        if(lineidx < file->getLineCount()) {
            if(curposition.lineidx == lineidx) {
                dcursRow+=i;
            }
            int pos = 1;
            for(auto c : file->getLine(lineidx)) {
                if(pos == width - 1) {
                    if(i == height - 3) break;
                    pos = 1;
                    i++;
                }
                wmove(win, i + 1, pos);
                waddch(win, c);
                pos++;
            }
            lineidx++;
            // wprintw(win, file->getLine(i).c_str());
        } else {
            wmove(win, i + 1, 1);
            if(coloured) wattron(win, COLOR_PAIR(EMPTYROW));
            waddch(win, '~');
            if(coloured) wattroff(win, COLOR_PAIR(EMPTYROW));
        }
    }
    dcursRow += file->getCursor().charidx / (width - 2); // account for 2 sides of border
    dcursCol += file->getCursor().charidx % (width - 2);
    wmove(win, dcursRow, dcursCol);
    wrefresh(win);
};
