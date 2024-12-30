#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "operands.h"

int main()
{
    srand(time(NULL));
    system("clear");
    int width, height, mines;
    bool validPick = false;
    int x;

    do{
        printf("Podaj poziom trudności: 1 - łatwy; 2 - średni; 3 - trudny\n");
        if(scanf("%d", &x) == 1)
        {
            if(x >=1 && x <=3)
            validPick = true;
        }
        system("clear");   
    }while(!validPick);
    
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

    tile **playField = createTiles(width, height);


    placeMines(width, height, mines, playField);

    printBoard(height, width, playField);

    revealMines(width, height, playField);

    for(int i = 0; i < height; i++)
    {
        free(playField[i]);
    }
    free(playField);
    
}


