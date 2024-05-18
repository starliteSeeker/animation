#include "ripple.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _ripple
{
    int y, x; // center
    int dist; // dist from center
    struct _ripple *next;
} Ripple;

void draw(Ripple *r, char c);

void ripple(void)
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    timeout(0);
    keypad(stdscr, true);
    mousemask(BUTTON1_PRESSED, NULL);
    refresh();

    mvaddstr(5, 10, "qwewq");

    Ripple *head = NULL;
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
                        Ripple *temp = malloc(sizeof(Ripple));
                        temp->y = event.y;
                        temp->x = event.x;
                        temp->dist = 0;
                        temp->next = head;
                        head = temp;
                    }
                }
                break;
            case 'q':
                quitFlag = true;
                break;
            }
        }
        if (quitFlag)
            break;

        // erase previous ripples
        Ripple *curr = head;
        while (curr != NULL)
        {
            draw(curr, ' ');
            curr = curr->next;
        }
        curr = head;
        Ripple *prev = NULL;
        while (curr != NULL)
        {
            if (curr->dist > LINES + COLS)
            {
                // delete curr
                if (prev == NULL)
                {
                    head = curr->next;
                    free(curr);
                    curr = head;
                }
                else
                {
                    prev->next = curr->next;
                    free(curr);
                    curr = prev->next;
                }
            }
            else
            {
                // draw curr
                curr->dist++;
                draw(curr, '*');
                prev = curr;
                curr = curr->next;
            }
        }

        usleep(50000);
    }

    Ripple *curr = head;
    while (curr != NULL)
    {
        Ripple *temp = curr->next;
        free(curr);
        curr = temp;
    }
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
    refresh();
}
