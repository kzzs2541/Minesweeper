#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "operands.h"

int main()
{
    srand(time(NULL));
    system("clear");
    int width, height, mines, currentScore, multiplier;
    bool validPick = false;
    int mode;

    do
    {
        printf("Podaj poziom trudności: 1 - łatwy; 2 - średni; 3 - trudny; 4 - custom\n");
        if (scanf("%d", &mode) == 1)
        {
            if (mode >= 1 && mode <= 4)
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
        multiplier = 1;
    }
    else if (mode == 2)
    {
        width = 16;
        height = 16;
        mines = 40;
        multiplier = 2;
    }
    else if (mode == 3)
    {
        width = 30;
        height = 16;
        mines = 99;
        multiplier = 3;
    }
    else
    {
        do
        {
            printf("Podaj wymiary planszy <szerokosc int> <wysokosc int> i ilosc min <int>\n");
            if (scanf("%d %d %d", &width, &height, &mines) == 3)
            {
                if (mines <= width * height)
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

        multiplier = 4; //do przemyslenia
    }

    tile **playField = createTiles(width, height);

    // pierwszy input - unikniecie odkrycia miny w pierwszym ruchu
    while (getchar() != '\n')
        ;
    char action;
    int x, y;
    do
    {
        printBoard(height, width, playField);
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

    while (getchar() != '\n')
        ;
    if (action == 'r')
    {
        revealEmptyTiles(width, height, playField, x - 1, y - 1);
    }

    while (checkGameStatus(width, height, mines, playField) == 0)
    {
        // dajemy ruch
        do
        {
            validPick = false;
            system("clear");
            currentScore = score(width, height, playField, multiplier);
            printf("Current score: %d\n", currentScore);
            printf("\n");
            printBoard(height, width, playField);
            printf("Podaj ruch: ");
            if (scanf("%c %d %d", &action, &x, &y) == 3)
            {
                if (action == 'f' && x > 0 && x <= width && y > 0 && y <= height && !playField[y - 1][x - 1].isRevealed)
                    validPick = true;
                else if (action == 'r' && x > 0 && x <= width && y > 0 && y <= height && !playField[y - 1][x - 1].isRevealed && !playField[y - 1][x - 1].isFlagged)
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
        while (getchar() != '\n')
            ;
        if (action == 'f')
        {
            playField[y - 1][x - 1].isFlagged = !playField[y - 1][x - 1].isFlagged;
        }
        else if (action == 'r' && playField[y - 1][x - 1].minesAround == 0)
        {
            playField[y - 1][x - 1].isRevealed = true;
            revealEmptyTiles(width, height, playField, x - 1, y - 1);
        }
        else
            playField[y - 1][x - 1].isRevealed = true;
    }
    currentScore = score(width, height, playField, multiplier);
    if (checkGameStatus(width, height, mines, playField) == 2)
    {
        revealMines(width, height, playField);
        printf("Game over\n");
        printf("Final score: %d\n", currentScore);
    }
    else if (checkGameStatus(width, height, mines, playField) == 1)
    {
        printBoard(height, width, playField);
        printf("Gratulacje!!!");
        printf("Final score: %d\n", currentScore);
    }

    for (int i = 0; i < height; i++)
    {
        free(playField[i]);
    }
    free(playField);
}
