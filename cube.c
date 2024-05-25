#include "cube.h"
#include <curses.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define PI 3.14
#define LIGHTS ".':;-=+*vTZE#M@"
#define LIGHTS_COUNT (sizeof LIGHTS - 1)

typedef struct
{
    double x, y, z;
} Vector;

// cube centered at (0, 0, 0), rotation around (1, 1, -1)
void cube()
{
    initscr();
    noecho();
    cbreak();
    int prev_curs = curs_set(0);
    timeout(0);
    refresh();

    double phi = 0;
    Vector k = {1 / sqrt(3), 1 / sqrt(3), -1 / sqrt(3)};
    double size = (COLS > LINES ? LINES : COLS) * 0.9;
    while (getch() == ERR)
    {
        clear();

        // calculate 3 vectors' new directions
        // then use them to draw the whole cube
        double sp = sin(phi), cp = 1 - cos(phi);
        // v1, v2, v3 are rotated from (-1, 0, 0), (0, -1, 0), (0, 0, 1)
        // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula#Matrix_notation
        Vector v1 = {-1 + (k.z * k.z + k.y * k.y) * cp, -k.z * sp - k.x * k.y * cp, k.y * sp - k.x * k.z * cp};
        Vector v2 = {k.z * sp - k.x * k.y * cp, -1 + (k.z * k.z + k.x * k.x) * cp, -k.x * sp - k.y * k.z * cp};
        Vector v3 = {k.y * sp + k.x * k.z * cp, -k.x * sp + k.y * k.z * cp, 1 - (k.y * k.y + k.x * k.x) * cp};

        // only draw faces that face front
        if (v1.z > 0)
        {
            int lightIdx = v1.z * LIGHTS_COUNT;
            char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
            for (double i = 0; i <= size; i += 0.5)
            {
                for (double j = 0; j <= size; j += 0.5)
                {
                    double x = -size / 2 - i * v2.x - j * v3.x;
                    double y = -size / 2 - i * v2.y - j * v3.y;
                    // double z = size / 2 - i * v2.z - j * v3.z;

                    int xProject = x + (double)COLS / 2;
                    int yProject = -y / 2 + (double)LINES / 2;
                    mvaddch(yProject, xProject, light);
                }
            }
        }
        else if (v1.z < 0)
        {
            int lightIdx = -v1.z * LIGHTS_COUNT;
            char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
            for (double i = 0; i <= size; i += 0.5)
            {
                for (double j = 0; j <= size; j += 0.5)
                {
                    double x = size / 2 + i * v2.x + j * v3.x;
                    double y = size / 2 + i * v2.y + j * v3.y;
                    // double z = -size / 2 + i * v2.z + j * v3.z;

                    int xProject = x + (double)COLS / 2;
                    int yProject = -y / 2 + (double)LINES / 2;
                    mvaddch(yProject, xProject, light);
                }
            }
        }
        if (v2.z > 0)
        {
            int lightIdx = v2.z * LIGHTS_COUNT;
            char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
            for (double i = 0; i <= size; i += 0.5)
            {
                for (double j = 0; j <= size; j += 0.5)
                {
                    double x = -size / 2 - i * v1.x - j * v3.x;
                    double y = -size / 2 - i * v1.y - j * v3.y;
                    // double z = size / 2 - i * v1.z - j * v3.z;

                    int xProject = x + (double)COLS / 2;
                    int yProject = -y / 2 + (double)LINES / 2;
                    mvaddch(yProject, xProject, light);
                }
            }
        }
        else if (v2.z < 0)
        {
            int lightIdx = -v2.z * LIGHTS_COUNT;
            char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
            for (double i = 0; i <= size; i += 0.5)
            {
                for (double j = 0; j <= size; j += 0.5)
                {
                    double x = size / 2 + i * v1.x + j * v3.x;
                    double y = size / 2 + i * v1.y + j * v3.y;
                    // double z = -size / 2 + i * v1.z + j * v3.z;

                    int xProject = x + (double)COLS / 2;
                    int yProject = -y / 2 + (double)LINES / 2;
                    mvaddch(yProject, xProject, light);
                }
            }
        }
        if (v3.z > 0)
        {
            int lightIdx = v3.z * LIGHTS_COUNT;
            char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
            for (double i = 0; i <= size; i += 0.5)
            {
                for (double j = 0; j <= size; j += 0.5)
                {
                    double x = -size / 2 - i * v1.x - j * v2.x;
                    double y = -size / 2 - i * v1.y - j * v2.y;
                    // double z = size / 2 - i * v1.z - j * v2.z;

                    int xProject = x + (double)COLS / 2;
                    int yProject = -y / 2 + (double)LINES / 2;
                    mvaddch(yProject, xProject, light);
                }
            }
        }
        else if (v3.z < 0)
        {
            int lightIdx = -v3.z * LIGHTS_COUNT;
            char light = LIGHTS[lightIdx >= LIGHTS_COUNT ? LIGHTS_COUNT - 1 : lightIdx];
            for (double i = 0; i <= size; i += 0.5)
            {
                for (double j = 0; j <= size; j += 0.5)
                {
                    double x = size / 2 + i * v1.x + j * v2.x;
                    double y = size / 2 + i * v1.y + j * v2.y;
                    // double z = -size / 2 + i * v1.z + j * v2.z;

                    int xProject = x + (double)COLS / 2;
                    int yProject = -y / 2 + (double)LINES / 2;
                    mvaddch(yProject, xProject, light);
                }
            }
        }
        refresh();

        // turn cube
        phi += PI * 2 / 60;
        usleep(50000);
    }

    curs_set(prev_curs);
    endwin();
}
