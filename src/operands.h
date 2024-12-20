#ifndef _OPERANDS_H_
#define _OPERANDS_H_

#include <stdbool.h>

typedef struct tile{
    int x, y;
    bool isFlagged;
    bool isMine;
    int minesAround;
}tile;

tile *createTiles(int width, int height);

#endif