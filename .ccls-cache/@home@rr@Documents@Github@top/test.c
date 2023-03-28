#include <ncurses.h>
#include <unistd.h>

int main() {
    int ch = 0;

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    while ((ch = getch()) != 113) {
        clear();
        mvprintw(8,10,"test");
        refresh();
        sleep(2);
    }
    endwin();
    return 0;
}
