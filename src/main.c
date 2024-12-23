#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "operands.h"

int main(int argc, char **argv)
{
    system("clear");
    int width, height, mines;
    printf("Podaj poziom trudności: 1 - łatwy; 2 - średni; 3 - trudny\n");
    int x;
    scanf("%d", &x);
    while(!(x == 1 || x == 2 || x == 3))
    {
        system("clear");
        printf("Podaj poziom trudności: 1 - łatwy; 2 - średni; 3 - trudny\n");
        scanf("%d", &x);
    }
    if(x == 1)
    {
        width = 9;
        height = 9;
        mines = 10;
    }
    else if(x == 2)
    {
        width = 16;
        height = 16;
        mines = 40;
    }
    else
    {
        width = 30;
        height = 16;
         mines = 99;
    }

    tile *playField = createTiles(width, height);

    free(playField);
    
}


