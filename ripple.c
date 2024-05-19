#include "ripple.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_CAP 40

typedef struct _ripple
{
    bool enabled;
    int y, x; // center
    int dist; // dist from center
} Ripple;

void draw(Ripple *r, char c);

Ripple arr[BUF_CAP] = {0};

void ripple(void)
{
    initscr();
    noecho();
    cbreak();
    int prev_curs = curs_set(0);
    nodelay(stdscr, true);
    keypad(stdscr, true);
    mouseinterval(0);
    // all mouse event is needed or else animation gets stuck
    mousemask(ALL_MOUSE_EVENTS, NULL);
    refresh();

    mvaddstr(0, 0, "click on terminal");
    mvaddstr(1, 0, "any key to quit");

    MEVENT event;
    int ch;
    bool quitFlag = false;
    for (;;)
    {
        // add new ripples
        while ((ch = getch()) != ERR)
        {
            switch (ch)
            {
            case KEY_MOUSE:
                if (getmouse(&event) == OK)
                {
                    if (event.bstate & BUTTON1_PRESSED)
                    {
                        int i;
                        for (i = 0; i < BUF_CAP; i++)
                        {
                            if (!arr[i].enabled)
                            {
                                arr[i].y = event.y;
                                arr[i].x = event.x;
                                arr[i].dist = 0;
                                arr[i].enabled = true;
                                break;
                            }
                        }
                    }
                }
                break;
            default:
                quitFlag = true;
                break;
            }
        }
        if (quitFlag)
            break;

        // erase previous ripples
        for (int i = 0; i < BUF_CAP; i++)
        {
            if (arr[i].enabled)
                draw(arr + i, ' ');
        }
        for (int i = 0; i < BUF_CAP; i++)
        {
            if (!arr[i].enabled)
                continue;

            if (arr[i].dist > LINES + COLS)
            {
                // delete curr
                arr[i].enabled = false;
            }
            else
            {
                // draw curr
                arr[i].dist++;
                draw(arr + i, '*');
            }
        }
        refresh();
        usleep(50000);
    }

    curs_set(prev_curs);
    endwin();
}

void draw(Ripple *r, char c)
{
    if (r->dist == 0)
        mvaddch(r->y, r->x, c);
    else
    {
        int y1 = r->y - r->dist, x1 = r->x;
        int y2 = r->y, x2 = r->x - r->dist;
        int y3 = r->y + r->dist, x3 = r->x;
        int y4 = r->y, x4 = r->x + r->dist;
        for (int i = 0; i < r->dist; i++)
        {
            if (y1 >= 0 && y1 < LINES && x1 >= 0 && x1 < COLS)
                mvaddch(y1, x1, c);
            y1++;
            x1++;
            if (y2 >= 0 && y2 < LINES && x2 >= 0 && x2 < COLS)
                mvaddch(y2, x2, c);
            y2--;
            x2++;
            if (y3 >= 0 && y3 < LINES && x3 >= 0 && x3 < COLS)
                mvaddch(y3, x3, c);
            y3--;
            x3--;
            if (y4 >= 0 && y4 < LINES && x4 >= 0 && x4 < COLS)
                mvaddch(y4, x4, c);
            y4++;
            x4--;
        }
    }
}
