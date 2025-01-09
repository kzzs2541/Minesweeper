#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "operands.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    system("clear");
    int width, height, mines, currentScore, multiplier;
    bool validPick = false;
    int mode;

    int opt;
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            filename = optarg;
            break;
        default:
            fprintf(stderr, "Poprawne użycie: %s [-f <filename>]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (filename)
    {
        printf("Wykonywanie symulacji ruchów...\n");

        int gameStatus;
        tile **playField = getBoard(filename, &width, &height, &mines, &gameStatus);

        if (!playField)
            return EXIT_FAILURE;

        if (gameStatus == 1)
            printf("\nGratulacje! Dla podanych ruchów plansza została rozwiązana.\n");
        else if (gameStatus == 2)
            printf("\nPorażka! Dla podanych ruchów gra zakończyła się porażką.\n");
        else
            printf("\nDla podanych ruchów gra się nie zakończyła.\n");

        // printf("Stan planszy po wykonaniu wszystkich ruchów z pliku:\n\n");
        // printBoard(height,width,playField);

        for (int i = 0; i < height; i++)
            free(playField[i]);
        free(playField);
    }
    else // tryb gracza
    {

        do
        {
            printf("Podaj poziom trudności: 1 - łatwy; 2 - średni; 3 - trudny; 4 - custom\n"); // podanie poziomu trudnosci gracza - do momentu az poda wlasciwy wybor
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

        if (mode == 1) // zapisanie ustawien planszy dla danego poziomu trudności
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
        else // customowa plansza - nie liczona do wynikow
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

            multiplier = 0;
        }

        tile **playField = createTiles(width, height); // inicjalizacja pola gry

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

        // operacje pierwszego ruchu - miny nie są jeszcze wygenerowane, wiec nie natrafimy w pierwszym ruchu na mine
        if (action == 'f') //flagowanie pola
        {
            playField[y - 1][x - 1].isFlagged = !playField[y - 1][x - 1].isFlagged;
        }
        else if (action == 'r') //odkrycie pola
        {
            playField[y - 1][x - 1].isRevealed = !playField[y - 1][x - 1].isRevealed;
        }
        placeMines(width, height, mines, playField, x - 1, y - 1); //generowanie min na planszy

        while (getchar() != '\n')
            ;
        if (action == 'r') //jezeli pierwsza akcja to odkrycie pola to rekurencyjnie odkrywamy puste pola wokol
        {
            revealEmptyTiles(width, height, playField, x - 1, y - 1);
        }

        while (checkGameStatus(width, height, mines, playField) == 0) //rozpoczęcie wlasciwej rozgrywki - do momentu az nie natrafimy na minę lub nie odkryjemy wszystkich pol
        {
            // dajemy ruch
            do
            {
                validPick = false;
                system("clear");
                currentScore = score(width, height, playField, multiplier);
                printf("Current score: %d\n", currentScore); //wyswietlenie aktualnego wyniku gracza
                printf("\n");
                printBoard(height, width, playField); //wyswietlenie aktualnego stanu planszy
                printf("Podaj ruch: ");
                if (scanf("%c %d %d", &action, &x, &y) == 3) //czytanie planszy do momentu otrzymania prawidlowego wejscia
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
            //wykonanie operacji na planszy
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

        currentScore = score(width, height, playField, multiplier); //obliczenie ostatecznego wyniku gracza
        if (checkGameStatus(width, height, mines, playField) == 2) //rezultat gracza po natrafieniu na mine
        {
            revealMines(width, height, playField); //wyswietlenie planszy z ujawnionymi pozycjami min
            printf("Game over\n");
            if(mode != 4)
                endGame(currentScore);
            else 
                displayLeaderboard();
        }
        else if (checkGameStatus(width, height, mines, playField) == 1)
        {
            printBoard(height, width, playField);
            printf("Gratulacje!!!\n");
            if(mode != 4)
                endGame(currentScore);
            else
                displayLeaderboard();
        }

        for (int i = 0; i < height; i++)
        {
            free(playField[i]);
        }
        free(playField);
    }
}
