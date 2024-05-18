#include "bounce.h"
#include <curses.h>
#include <stdio.h>

void bounce()
{
    initscr();
    noecho();
    cbreak();
    int prev_curs = curs_set(0);
    timeout(50); // timeout for getch()

    int x = COLS / 2;
    int y = LINES / 2;
    bool down = true, right = true;
    int ch;

    while ((ch = getch()) == ERR) // while no key is pressed
    {
        // erase previous frame
        mvaddch(y, x, ' ');

        // move ball
        if (x >= COLS - 1 || x <= 0)
        {
            right = !right;
        }
        if (y >= LINES - 1 || y <= 0)
        {
            down = !down;
        }
        x += right ? 1 : -1;
        y += down ? 1 : -1;

        // draw new frame
        mvaddch(y, x, 'O');
        refresh();
    }

    curs_set(prev_curs);
    endwin();
}
