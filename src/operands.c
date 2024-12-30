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

void printFullBoard(int height, int width, tile **playField)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (playField[i][j].isMine)
            {
                printf("X ");
            }
            else if (playField[i][j].minesAround == 0)
            {
                printf("□ ");
            }
            else if (playField[i][j].minesAround != 0)
            {
                printf("%d ", playField[i][j].minesAround);
            }
            
        }
        printf("\n");
    }
}

void printBoard(int height, int width, tile **playField) //to do: naprawic wyswietlanie wspolrzednych - rozjezdza sie przy liczbach dwucyfrowych
{
    printf("  ");
    for(int i = 1; i <= width; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < height; i++)
    {
        printf("%d", i);
        for (int j = 0; j < width; j++)
        {
            if (!playField[i][j].isRevealed && playField[i][j].isFlagged)
            {
                printf(" F");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround == 0)
            {
                printf(" □");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf(" %d", playField[i][j].minesAround);
            }
            else if (!playField[i][j].isRevealed)
            {
                printf(" ■");
            }
        }
        printf("\n");
    }
}

void placeMines(int width, int height, int mines, tile **playField, int x, int y)
{
    int placedMines = 0;
    int pickX, pickY;
    while (placedMines < mines)
    {
        bool canBePlaced = true;
        pickX = rand() % width;
        pickY = rand() % height;
        if (!playField[pickY][pickX].isMine && pickX != x && pickY != y)
        {
            playField[pickY][pickX].isMine = true;
            placedMines++;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (!playField[i][j].isMine)
            {
                // rogi (warunki zeby zapobiec wyjsciu poza obszar tablicy)
                if (i == 0 && j == 0)
                {
                    for (int k = 0; k <= 1; k++)
                    {
                        for (int l = 0; l <= 1; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else if (i == 0 && j == width - 1)
                {
                    for (int k = 0; k <= 1; k++)
                    {
                        for (int l = -1; l <= 0; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else if (i == height - 1 && j == 0)
                {
                    for (int k = -1; k <= 0; k++)
                    {
                        for (int l = 0; l <= 1; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else if (i == height - 1 && j == width - 1)
                {
                    for (int k = -1; k <= 0; k++)
                    {
                        for (int l = -1; l <= 0; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                // krawedzie
                else if (i == 0)
                {
                    for (int k = 0; k <= 1; k++)
                    {
                        for (int l = -1; l <= 1; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else if (i == height - 1)
                {
                    for (int k = -1; k <= 0; k++)
                    {
                        for (int l = -1; l <= 1; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else if (j == 0)
                {
                    for (int k = -1; k <= 1; k++)
                    {
                        for (int l = 0; l <= 1; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else if (j == width - 1)
                {
                    for (int k = -1; k <= 1; k++)
                    {
                        for (int l = -1; l <= 0; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
                else
                {
                    for (int k = -1; k <= 1; k++)
                    {
                        for (int l = -1; l <= 1; l++)
                        {
                            if (playField[i + k][j + l].isMine)
                            {
                                playField[i][j].minesAround++;
                            }
                        }
                    }
                }
            }
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
            else if (!playField[i][j].isRevealed && playField[i][j].isFlagged)
            {
                printf("F ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround == 0)
            {
                printf("□ ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf("%d ", playField[i][j].minesAround);
            }
            else if (!playField[i][j].isRevealed)
            {
                printf("■ ");
            }
        }
        printf("\n");
    }
}

int checkGameStatus(int width, int height, int mines, tile **playField)
{
    int tilesRevealed = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (playField[i][j].isRevealed)
                tilesRevealed++;

            if (playField[i][j].isRevealed && playField[i][j].isMine)
                return 2; // odkryto mine - przegrana
        }
    }
    if (tilesRevealed == width * height - mines)
    {
        return 1; // wygrana
    }
    if (tilesRevealed < width * height - mines)
    {
        return 0; // gra sie nie ksonczyla - kontynuuj
    }
}
