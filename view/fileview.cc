#include "fileview.h"
#include "../model/filestate.h"
#include <ncurses.h>

#define EMPTYROW 1

FileView::FileView(FileState *file): file{file} {
    width = getmaxx(stdscr);
    height = getmaxy(stdscr) - 3;
    win = newwin(height, width, 0, 0);
    if(has_colors()) {
        init_pair(EMPTYROW, COLOR_MAGENTA, COLOR_BLACK);
        coloured = true;
    }
    refresh();
    displayView();
}

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
        // need to find the first line that fits in the screen while the cursor 
        // is still visible
        curOffset++;

        // TODO: handle edge case where 1 line may span the entire screen
        while(curCursorLine >= getActualDisplayLine(curOffset, 0) + height - 2) {
            curOffset++;
        }
    }
}

void FileView::displayView() {
    werase(win);
    box(win, 0, 0);

    if(file->getForcedPageMove() != 0) {
        // move curOffset
        // then move cursor

        // Cursor pos = file->getCursor();
        int advance = file->getForcedPageMove() * (height-3)/2;
        int offsetchange = 0;
        // move down by 1 page
        if(advance > 0) {
            while(advance > 0) {
                if(curOffset + offsetchange == file->getLineCount()) {
                    offsetchange--; // don't go past end of file
                    break;
                }
                int linesize = file->getLine(curOffset + offsetchange).size();
                if(linesize == 0) {
                    advance--;
                } else {
                    int lines = linesize / (width - 2);
                    if(linesize % (width - 2) != 0) {
                        lines++;
                    }
                    if(advance >= lines) {
                        advance -= lines;
                    } else {
                        break;
                    }
                }
                offsetchange++;
            }
            curOffset += offsetchange;
            if(file->getForcedPageMove() == 1) {
                file->moveCursor(0, offsetchange, true);
            } else {
                file->setCursor(Cursor{curOffset, 0});
            }
        }
        else {
            while(advance < 0) {
                if(curOffset - offsetchange == 0) {
                    // don't go past end of file
                    break;
                }
                int linesize = file->getLine(curOffset - offsetchange).size();
                if(linesize == 0) {
                    advance++;
                } else {
                    int lines = linesize / (width - 2);
                    if(linesize % (width - 2) != 0) {
                        lines++;
                    }
                    if(advance < lines) {
                        advance += lines;
                    } else {
                        break;
                    }
                }
                offsetchange++;
            }
            curOffset -= offsetchange;
            if(file->getForcedPageMove() == -1) {
                file->moveCursor(0, -offsetchange, true);
            } else {
                file->setCursor(Cursor{curOffset, 0});
            }
        }

        // reset forced offset back to 0
        file->setForcedPageMove(0);
    } else {
        updateWindow();
    }

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
