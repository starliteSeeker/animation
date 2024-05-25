#include "bounce.h"
#include "coin.h"
#include "combination_lock.h"
#include "cube.h"
#include "ripple.h"
#include "snow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHelp(char *name);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "bounce") == 0)
    {
        bounce();
    }
    else if (strcmp(argv[1], "snow") == 0)
    {
        snow();
    }
    else if (strcmp(argv[1], "combination_lock") == 0)
    {
        int size = 3;
        if (argc >= 3)
            size = atoi(argv[2]);
        combination_lock(size);
    }
    else if (strcmp(argv[1], "ripple") == 0)
    {
        ripple();
    }
    else if (strcmp(argv[1], "coin") == 0)
    {
        coin();
    }
    else if (strcmp(argv[1], "cube") == 0)
    {
        cube();
    }
    else
    {
        printHelp(argv[0]);
    }
    return EXIT_SUCCESS;
}

void printHelp(char *name)
{
    printf("%s <animation name>\n\n", name);
    printf("Available names:\n");
    printf("bounce\n");
    printf("snow\n");
    printf("combination_lock [size=3]\n");
    printf("ripple\n");
    printf("coin\n");
    printf("cube\n");
}
