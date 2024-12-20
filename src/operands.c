#include "operands.h"
#include <stdbool.h>
#include <stdlib.h>

tile *createTiles(int width, int height)
{
    tile* playField = malloc(width * height * sizeof(tile));
    for(int i = 1; i <= height; i++)
    {
        for(int j = 1; j <= width; j++)
        {
            playField[(i-1) * width + (j-1)].x = j; 
            playField[(i-1) * width + (j-1)].y = i;
            playField[(i-1) * width + (j-1)].isFlagged = false;
            playField[(i-1) * width + (j-1)].isMine = false;
            playField[(i-1) * width + (j-1)].minesAround = 0;
        }
    }
    return playField;
}