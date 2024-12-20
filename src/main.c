#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "operands.h"




int main(int argc, char **argv)
{
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    tile *playField = createTiles(width, height);

    free(playField);
    
}


