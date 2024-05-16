#include "bounce.h"
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
    else
    {
        printHelp(argv[0]);
    }
    return EXIT_SUCCESS;
}

void printHelp(char *name)
{
    printf("%s <animation name>\n", name);
}
