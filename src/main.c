#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "operands.h"

int main()
{
    srand(time(NULL));
    system("clear");
    int width, height, mines, flags;
    bool validPick = false;
    int mode;
    int gameStatus = 0;

    do
    {
        printf("Podaj poziom trudności: 1 - łatwy; 2 - średni; 3 - trudny\n");
        if (scanf("%d", &mode) == 1)
        {
            if (mode >= 1 && mode <= 3)
                validPick = true;
        }
        else
        {
            while (getchar() != '\n')
                ;
        }
        system("clear");
    } while (!validPick);
    validPick = false;

    if (mode == 1)
    {
        width = 9;
        height = 9;
        mines = 10;
        flags = 10;
    }
    else if (mode == 2)
    {
        width = 16;
        height = 16;
        mines = 40;
        flags = 40;
    }
    else
    {
        width = 30;
        height = 16;
        mines = 99;
        flags = 99;
    }

    tile **playField = createTiles(width, height);

    // pierwszy input - unikniecie odkrycia miny w pierwszym ruchu
    while (getchar() != '\n')
        ;
    char action;
    int x, y;
    do
    {
        printf("Podaj ruch: ");
        if (scanf("%c %d %d", &action, &x, &y) == 3)
        {
            if ((action == 'f' || action == 'r') && x > 0 && x <= width && y > 0 && y <= height)
                validPick = true;
            else
            {
                while (getchar() != '\n')
                    ;
            }
        }
        else
        {
            while (getchar() != '\n')
                ;
        }
        system("clear");
    } while (!validPick);

    if (action == 'f')
    {
        playField[y - 1][x - 1].isFlagged = !playField[y - 1][x - 1].isFlagged;
    }
    else if (action == 'r')
    {
        playField[y - 1][x - 1].isRevealed = !playField[y - 1][x - 1].isRevealed;
    }
    placeMines(width, height, mines, playField, x - 1, y - 1);

    printBoard(height, width, playField);

    // printFullBoard(height, width, playField);

    //printf("%d", checkGameStatus(width, height, mines, playField));

    for (int i = 0; i < height; i++)
    {
        free(playField[i]);
    }
    free(playField);
}
