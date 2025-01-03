#include "operands.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX_NAME_LENGTH 20
#define MAX_TOP_SCORES 5
#define LEADERBOARD_FILE "leaderboard.txt"

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

void printBoard(int height, int width, tile **playField) // to do: naprawic wyswietlanie wspolrzednych - rozjezdza sie przy liczbach dwucyfrowych
{
    printf("  ");
    for (int i = 1; i <= width; i++)
    {
        if (i > 10)
            printf("%d ", i);
        else
            printf(" %d ", i);
    }
    printf("\n");
    for (int i = 0; i < height; i++)
    {
        printf("%d", i + 1);
        if (i < 9)
        {
            printf(" ");
        }
        for (int j = 0; j < width; j++)
        {
            if (!playField[i][j].isRevealed && playField[i][j].isFlagged)
            {
                printf(" X ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround == 0)
            {
                printf(" â–ˇ ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf(" %d ", playField[i][j].minesAround);
            }
            else if (!playField[i][j].isRevealed)
            {
                printf(" â–  ");
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
                else // reszta pol
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
    printf("  ");
    for (int i = 1; i <= width; i++)
    {
        if (i > 10)
            printf("%d ", i);
        else
            printf(" %d ", i);
    }
    printf("\n");
    for (int i = 0; i < height; i++)
    {
        printf("%d", i + 1);
        if (i < 9)
        {
            printf(" ");
        }
        for (int j = 0; j < width; j++)
        {
            if (playField[i][j].isMine)
            {
                printf(" M ");
            }
            else if (!playField[i][j].isRevealed && playField[i][j].isFlagged)
            {
                printf(" X ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround == 0)
            {
                printf(" â–ˇ ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf(" %d ", playField[i][j].minesAround);
            }
            else if (!playField[i][j].isRevealed)
            {
                printf(" â–  ");
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
        return 0; // gra sie nie skonczyla - kontynuuj
    }
}

void revealEmptyTiles(int width, int height, tile **playField, int x, int y)
{
    if (!playField[y][x].isMine)
    {
        // gorny
        if (y - 1 >= 0 && !playField[y - 1][x].isMine && !playField[y - 1][x].isRevealed)
        {
            if (playField[y - 1][x].minesAround > 0)
            {
                playField[y - 1][x].isRevealed = true;
            }
            else
            {
                playField[y - 1][x].isRevealed = true;
                revealEmptyTiles(width, height, playField, x, y - 1);
            }
        }
        // prawy
        if (x + 1 < width && !playField[y][x + 1].isMine && !playField[y][x + 1].isRevealed)
        {
            if (playField[y][x + 1].minesAround > 0)
            {
                playField[y][x + 1].isRevealed = true;
            }
            else
            {
                playField[y][x + 1].isRevealed = true;
                revealEmptyTiles(width, height, playField, x + 1, y);
            }
        }
        // dolny
        if (y + 1 < height && !playField[y + 1][x].isMine && !playField[y + 1][x].isRevealed)
        {
            if (playField[y + 1][x].minesAround > 0)
            {
                playField[y + 1][x].isRevealed = true;
            }
            else
            {
                playField[y + 1][x].isRevealed = true;
                revealEmptyTiles(width, height, playField, x, y + 1);
            }
        }
        // lewy
        if (x - 1 >= 0 && !playField[y][x - 1].isMine && !playField[y][x - 1].isRevealed)
        {
            if (playField[y][x - 1].minesAround > 0)
            {
                playField[y][x - 1].isRevealed = true;
            }
            else
            {
                playField[y][x - 1].isRevealed = true;
                revealEmptyTiles(width, height, playField, x - 1, y);
            }
        }
        // lewy gorny
        if (x - 1 >= 0 && y - 1 >= 0 && !playField[y - 1][x - 1].isMine && !playField[y - 1][x - 1].isRevealed)
        {
            if (playField[y - 1][x - 1].minesAround > 0)
            {
                playField[y - 1][x - 1].isRevealed = true;
            }
        }
        // prawy gorny
        if (x + 1 < width && y - 1 >= 0 && !playField[y - 1][x + 1].isMine && !playField[y - 1][x + 1].isRevealed)
        {
            if (playField[y - 1][x + 1].minesAround > 0)
            {
                playField[y - 1][x + 1].isRevealed = true;
            }
        }
        // lewy dolny
        if (x - 1 >= 0 && y + 1 < height && !playField[y + 1][x - 1].isMine && !playField[y + 1][x - 1].isRevealed)
        {
            if (playField[y + 1][x - 1].minesAround > 0)
            {
                playField[y + 1][x - 1].isRevealed = true;
            }
        }
        // prawy dolny
        if (x + 1 < width && y + 1 < height && !playField[y + 1][x + 1].isMine && !playField[y + 1][x + 1].isRevealed)
        {
            if (playField[y + 1][x + 1].minesAround > 0)
            {
                playField[y + 1][x + 1].isRevealed = true;
            }
        }
    }
}

int score(int width, int height, tile **playField, int multiplier)
{
    int score = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (playField[i][j].isRevealed)
            {
                score++;
            }
        }
    }
    return score * multiplier;
}

void endGame(int currentScore)
{
    char name[MAX_NAME_LENGTH];
    printf("Wynik końcowy: %d\n", currentScore);
    printf("\nPodaj swój nick: ");
    fgets(name, MAX_NAME_LENGTH, stdin);

    //usuniecie znaku nowej linii z podanego nicku
    size_t length = strlen(name);
    if (length > 0 && name[length - 1] == '\n')
    {
        name[length - 1] = '\0';
    }

    saveScore(name, currentScore);
    printf("Twój wynik został zapisany!\n");
    printf("\n");
    displayLeaderboard();
}

void saveScore(const char *name, int score)
{
    FILE *file = fopen(LEADERBOARD_FILE, "a");
    if (file == NULL)
    {
        perror("Nie udało się otworzyć pliku z wynikami");
        return;
    }
    fprintf(file, "%s %d\n", name, score);
    fclose(file);
}

void displayLeaderboard()
{
    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (file == NULL)
    {
        perror("Nie udało się otworzyć pliku z wynikami");
        return;
    }

    typedef struct
    {
        char name[MAX_NAME_LENGTH];
        int score;
    } ScoreLine;

    ScoreLine scores[100];
    int count = 0;

    while (fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2)
        count++;

    fclose(file);

    //Sortowanie wyników
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j].score < scores[j + 1].score)
            {
                ScoreLine temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    printf("Najlepsze %d Graczy:\n", MAX_TOP_SCORES);
    for (int i = 0; i < count && i < MAX_TOP_SCORES; i++)
    {
        printf("%d. %s » %dpkt\n", i + 1, scores[i].name, scores[i].score);
    }
}

tile **getBoard(const char *filename, int *width, int *height, int *mines, int *gameStatus)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Nie udało się otworzyć pliku z planszą.\n", filename);
        return NULL;
    }

    if (fscanf(file, "%d %d", width, height) != 2)
    {
        printf("Błąd podczas wczytywania rozmiarów planszy.\n");
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d", mines) != 1)
    {
        printf("Błąd podczas wczytywania liczby min.\n");
        fclose(file);
        return NULL;
    }

    tile **playField = createTiles(*width, *height);

    for (int i = 0; i < *height; i++)
    {
        for (int j = 0; j < *width; j++)
        {
            int isMine;
            fscanf(file, "%d", &isMine);
            if(isMine==1)
                playField[i][j].isMine = true;
            else
                playField[i][j].isMine = false;
        }
    }

    char action;
    int x, y;
    *gameStatus = 0;

    while (fscanf(file, " %c %d %d", &action, &x, &y) == 3)
    {
        if (action == 'f')
        {
            playField[y - 1][x - 1].isFlagged = !playField[y - 1][x - 1].isFlagged;
        }
        else if (action == 'r')
        {
            if (playField[y - 1][x - 1].isMine)
            {
                *gameStatus = 2; // Porażka
                break;
            }
            else if (playField[y - 1][x - 1].minesAround == 0)
            {
                playField[y - 1][x - 1].isRevealed = true;
                revealEmptyTiles(*width, *height, playField, x - 1, y - 1);
            }
            else
            {
                playField[y - 1][x - 1].isRevealed = true;
            }
        }

        int status = checkGameStatus(*width, *height, *mines, playField);
        if (status != 0)
        {
            *gameStatus = status;
            break;
        }
    }

    fclose(file);
    return playField;
}
