#include "operands.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

tile **createTiles(int width, int height)
{
    tile **playField = malloc(height * sizeof(tile *));
    for (int i = 0; i < height; i++)
    {
        playField[i] = malloc(width * sizeof(tile));
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            playField[i][j].isFlagged = false;
            playField[i][j].isMine = false;
            playField[i][j].minesAround = 0;
            playField[i][j].isRevealed = false;
        }
    }
    return playField;
}

void printBoard(int height, int width, tile **playField)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            /*if (playField[i][j].isMine)
            {
                printf("M ");
            }*/
            if (!playField[i][j].isRevealed)
            {
                printf("■ ");
            }
            if (playField[i][j].isRevealed && playField[i][j].minesAround == 0)
            {
                printf("□ ");
            }
            if (playField[i][j].isRevealed && playField[i][j].isFlagged)
            {
                printf("F ");
            }
            if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf("%d ", playField[i][j].minesAround);
            }
        }
        printf("\n");
    }
}

void placeMines(int width, int height, int mines, tile **playField)
{
    int placedMines = 0;
    int pickX, pickY;
    while (placedMines < mines)
    {
        bool canBePlaced = true;
        pickX = rand() % width;
        pickY = rand() % height;
        if (!playField[pickY][pickX].isMine)
        {
            playField[pickY][pickX].isMine = true;
            placedMines++;
        }
    }
}

void revealMines(int width, int height, tile **playField)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (playField[i][j].isMine)
            {
                printf("M ");
            }
            else if (!playField[i][j].isRevealed)
            {
                printf("■ ");
            }
            if (playField[i][j].isRevealed && playField[i][j].minesAround == 0)
            {
                printf("□ ");
            }
            if (playField[i][j].isRevealed && playField[i][j].isFlagged)
            {
                printf("F ");
            }
            if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf("%d ", playField[i][j].minesAround);
            }
        }
        printf("\n");
    }
}
