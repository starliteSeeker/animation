#include "coin.h"
#include <curses.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PI 3.14
#define LIGHTS ".':;-=+*vTZE#M@"
#define LIGHTS_COUNT (sizeof LIGHTS - 1)

// coin centered at (D, 0, 0), rotation axis parallel to y axis
void coin(void)
{
    initscr();
    noecho();
    cbreak();
    int prev_curs = curs_set(0);
    timeout(0);
    refresh();

    // angle of coin
    double phi = 0;
    // multiply by 2 to take character width and height into consideration
    double radius = (double)(LINES * 2 > COLS ? COLS / 2 : LINES) - 2;
    while (getch() == ERR)
    {
        // draw coin
        clear();
        // int lightIdx = fabs(cos(phi - PI / 6) + 1) * LIGHTS_COUNT / 2;
        // char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
        int lightIdx = sin(phi + PI / 3) * LIGHTS_COUNT;
        char light = LIGHTS[lightIdx < 0 ? 0 : lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
        for (double theta = 0; theta < PI * 2; theta += 0.5 / radius)
        {
            for (double r = 0; r < radius; r += 0.5)
            {
                // double x = DIST + r * sin(phi) * cos(theta);
                double y = r * sin(theta);
                double z = r * cos(phi) * cos(theta);

                int xProject = z + (double)COLS / 2;
                int yProject = -y / 2 + (double)LINES / 2;
                // check range just to be safe
                if (xProject >= 0 && xProject < COLS && yProject >= 0 && yProject < LINES)
                    mvaddch(yProject, xProject, light);
            }
        }

        refresh();

        // turn coin
        phi += 6.28 / 50;
        if (phi > PI / 2)
            phi -= PI;
        usleep(50000);
    }

    curs_set(prev_curs);
    endwin();
}
