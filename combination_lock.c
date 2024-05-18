#include "combination_lock.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void draw_lock_number(WINDOW *w, unsigned char num);

#define FONT_WIDTH 7
#define FONT_HEIGHT 7

char font[10][FONT_HEIGHT][FONT_WIDTH + 1] = {
    {
        "  000  ",
        " 0  00 ",
        "00   00",
        "00   00",
        "00   00",
        " 00  0 ",
        "  000  ",
    },
    {
        "   11  ",
        "  111  ",
        "   11  ",
        "   11  ",
        "   11  ",
        "   11  ",
        " 111111",
    },
    {
        " 22222 ",
        "22   22",
        "    222",
        "  2222 ",
        " 2222  ",
        "222    ",
        "2222222",
    },
    {
        " 333333",
        "    33 ",
        "   33  ",
        "  3333 ",
        "     33",
        "33   33",
        " 33333 ",
    },
    {
        "   444 ",
        "  4444 ",
        " 44 44 ",
        "44  44 ",
        "4444444",
        "    44 ",
        "    44 ",
    },
    {
        "555555 ",
        "55     ",
        "555555 ",
        "     55",
        "     55",
        "55   55",
        " 55555 ",
    },
    {
        "  6666 ",
        " 66    ",
        "66     ",
        "666666 ",
        "66   66",
        "66   66",
        " 66666 ",
    },
    {
        "7777777",
        "77   77",
        "    77 ",
        "   77  ",
        "  77   ",
        "  77   ",
        "  77   ",
    },
    {
        " 8888  ",
        "88   8 ",
        "888  8 ",
        " 8888  ",
        "8  8888",
        "8    88",
        " 88888 ",
    },
    {
        " 99999 ",
        "99   99",
        "99   99",
        " 999999",
        "     99",
        "    99 ",
        " 9999  ",
    },
};

void combination_lock(int size)
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    refresh();

    char msg[] = " wasd to move, q to quit";
    char msg_win[] = " you did it :), q to quit";

    // calculate positioning
    if (LINES <= FONT_HEIGHT + 2 || COLS < size * (FONT_WIDTH + 1) + 1 || COLS < sizeof(msg_win))
    {
        endwin();
        printf("terminal too small\n");
        return;
    }
    int voffset = (LINES - FONT_HEIGHT - 2) / 2;
    int hoffset = (COLS - 1 - size * (FONT_WIDTH + 1)) / 2;

    // draw lock
    unsigned char focus = size - 1;
    unsigned char *lockNumber = calloc(size, sizeof(char));
    unsigned char *lockAnswer = malloc(size * sizeof(char));
    WINDOW **lock = malloc(sizeof(WINDOW *) * size);
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        WINDOW *temp = newwin(FONT_HEIGHT + 2, FONT_WIDTH + 2, voffset, hoffset + i * (FONT_WIDTH + 1));
        box(temp, 0, 0);
        draw_lock_number(temp, lockNumber[i]);
        lock[i] = temp;

        lockAnswer[i] = rand() % 10;
    }

    // draw bottom text
    mvaddstr(LINES - 1, 0, msg);
    refresh();

    for (int i = 0; i < size; i++)
    {
        mvaddch(0, i, '0' + lockAnswer[i]);
    }
    refresh();

    bool quitFlag = false;
    int ch;
    while (memcmp(lockAnswer, lockNumber, size) != 0)
    {
        ch = getch();
        switch (ch)
        {
        case 'a':
            if (focus <= 0)
            {
                continue;
            }
            focus--;
            touchwin(lock[focus]);
            wrefresh(lock[focus]);
            break;
        case 'd':
            if (focus >= size - 1)
            {
                continue;
            }
            focus++;
            touchwin(lock[focus]);
            wrefresh(lock[focus]);
            break;
        case 'w': {
            unsigned char num1 = lockNumber[focus];
            unsigned char num2 = (lockNumber[focus] + 1) % 10;
            WINDOW *w = lock[focus];
            for (int i = 0; i < FONT_HEIGHT - 1; i++)
            {
                for (int y = 0; y < FONT_HEIGHT; y++)
                {
                    if (y <= i)
                    {
                        mvwaddstr(w, 1 + y, 1, font[num2][FONT_HEIGHT - i - 1 + y]);
                    }
                    else
                    {
                        mvwaddstr(w, 1 + y, 1, font[num1][y - i - 1]);
                    }
                }
                wrefresh(w);
                usleep(25000);
            }
            lockNumber[focus] = num2;
            draw_lock_number(lock[focus], num2);
            flushinp(); // avoid queueing up animations
            break;
        }
        case 's': {
            unsigned char num1 = lockNumber[focus];
            // +9 is same as -1, avoid negative `%` jank
            unsigned char num2 = (lockNumber[focus] + 9) % 10;
            WINDOW *w = lock[focus];
            for (int i = 0; i < FONT_HEIGHT - 1; i++)
            {
                for (int y = 0; y < FONT_HEIGHT; y++)
                {
                    if (i + y < FONT_HEIGHT - 1)
                    {
                        mvwaddstr(w, 1 + y, 1, font[num1][i + y + 1]);
                    }
                    else
                    {
                        mvwaddstr(w, 1 + y, 1, font[num2][i + y + 1 - FONT_HEIGHT]);
                    }
                }
                wrefresh(w);
                usleep(25000);
            }
            lockNumber[focus] = num2;
            draw_lock_number(lock[focus], num2);
            flushinp(); // avoid queueing up animations
            break;
        }
        case 'q':
            quitFlag = true;
        }
        if (quitFlag)
            break;
    }

    // ending
    if (!quitFlag)
    {
        // draw bottom text
        mvaddstr(LINES - 1, 0, msg_win);
        refresh();
        while (getch() != 'q')
            ;
    }

    free(lockNumber);
    free(lock);
    endwin();
}

void draw_lock_number(WINDOW *w, unsigned char num)
{
    for (int y = 0; y < FONT_HEIGHT; y++)
    {
        mvwaddstr(w, 1 + y, 1, font[num][y]);
    }
    wrefresh(w);
}
