#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _snow
{
    // fixed point Q24.8
    int y, x;
    int dy, dx;
    unsigned int vy, vx;
    struct _snow *next;
} Snow;

void snow()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    timeout(40); // timeout for getch()

    srand(time(NULL));
    Snow *head = NULL;
    Snow *curr = NULL;

    int snowCounter = 0;

    int ch;
    while ((ch = getch()) == ERR)
    {
        // clear last frame
        curr = head;
        while (curr != NULL)
        {
            Snow *next = curr->next;
            mvaddch(curr->y >> 8, curr->x >> 8, ' ');
            curr = next;
        }

        // update position
        curr = head;
        Snow *last = NULL;
        while (curr != NULL)
        {
            Snow *next = curr->next;
            curr->dx += curr->vx;
            curr->dy += curr->vy;
            // only update x when y is updated
            if (curr->dy >> 8 > 0)
            {
                curr->y += curr->dy & (~0xff);
                curr->dy &= 0xff;
                curr->x -= curr->dx & (~0xff);
                curr->dx &= 0xff;
            }

            if (curr->x < 0 || curr->y >> 8 >= LINES)
            {
                // curr is out of screen, delete curr
                if (last == NULL)
                {
                    head = next;
                    free(curr);
                    curr = NULL;
                }
                else
                {
                    last->next = next;
                    free(curr);
                    curr = last;
                }
            }
            last = curr;
            curr = next;
        }

        // create new snow
        snowCounter += COLS + LINES / 2;
        while (snowCounter >= 150)
        {
            snowCounter -= 150;
            Snow *next = malloc(sizeof(Snow));

            next->x = (rand() % (COLS + LINES / 2)) << 8;
            next->y = 0 << 8;
            next->dx = 0;
            next->dy = 0;
            next->vx = (0b1100000) + rand() % 0b100000;
            next->vy = (1 << 7) + rand() % 0b110000;
            next->next = head;
            head = next;
        }

        // draw snow
        curr = head;
        while (curr != NULL)
        {
            Snow *next = curr->next;
            if (curr->x >> 8 < COLS && curr->y >= 0)
                mvaddch(curr->y >> 8, curr->x >> 8, '*');
            curr = next;
        }
    }

    curr = head;
    while (curr != NULL)
    {
        Snow *next = curr->next;
        free(curr);
        curr = next;
    }

    endwin();
}
