#include <iostream>
#include "ncurses.h"

#define ctrl(x) ((x) & 0x1f)

using std::cout, std::endl;

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    WINDOW *win = newwin(maxy - 5, maxx, 0, 0);
    WINDOW *cmdbar = newwin(5, maxx, maxy - 5, 0);
    box(win, 0, 0);
    box(cmdbar, 0, 0);
    refresh();
    wmove(cmdbar, 1, 1);
    wmove(win, 1, 1);
    wrefresh(cmdbar);
    wrefresh(win);
    while(true) {
        int ch = getch();
        if(ch == ctrl('c')) {
            cout << "\a";
            break;
        }
        if(ch == KEY_BACKSPACE) {
            wmove(win, getcury(win), getcurx(win) - 1);
            waddch(win, ' ');
            wmove(win, getcury(win), getcurx(win) - 1);
            wrefresh(win);
            continue;
        }
        waddch(cmdbar, ch);
        wrefresh(cmdbar);
        waddch(win, ch);
        wrefresh(win);
    }
    endwin();
    return 0;
}
