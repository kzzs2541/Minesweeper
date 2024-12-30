#ifndef _OPERANDS_H_
#define _OPERANDS_H_

#include <stdbool.h>

typedef struct tile{
    bool isFlagged;
    bool isMine;
    bool isRevealed;
    int minesAround;
}tile;

tile **createTiles(int width, int height);

void printBoard(int height, int width, tile **playField);

void placeMines(int width, int height, int mines, tile **playField);

void revealMines(int width, int height, tile **playField);

#endif