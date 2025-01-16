#include "operands.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX_NAME_LENGTH 20
#define MAX_TOP_SCORES 5
#define LEADERBOARD_FILE "src/leaderboard.txt"

tile **createTiles(int width, int height) // struktura planszy
{
    tile **playField = malloc(height * sizeof(tile *)); // rezerwowanie pamieci dla pol
    for (int i = 0; i < height; i++)
    {
        playField[i] = malloc(width * sizeof(tile));
    }

    for (int i = 0; i < height; i++) // inicjalizacja pol - czysta plansza
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

void printBoard(int height, int width, tile **playField) // funkcja wyswietlająca aktualny stan planszy
{
    printf("  "); // numeracja kolumn
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
            if (!playField[i][j].isRevealed && playField[i][j].isFlagged) // wyswietlanie oflagowanych pol
            {
                printf(" X ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround == 0) // wyswietlanie pustych odkrytych pol
            {
                printf(" □ ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround != 0) // wyswietlanie odkrytych pol wokol min
            {
                printf(" %d ", playField[i][j].minesAround);
            }
            else if (!playField[i][j].isRevealed) // wyswietlanie nieodkrytych pol
            {
                printf(" ■ ");
            }
        }
        printf("\n");
    }
}

void placeMines(int width, int height, int mines, tile **playField, int x, int y) // funkcja inicjalizująca miny
{
    int placedMines = 0;
    int pickX, pickY;
    while (placedMines < mines) // umieszczanie min losowo
    {
        pickX = rand() % width;
        pickY = rand() % height;
        if (!playField[pickY][pickX].isMine && pickX != x && pickY != y)
        {
            playField[pickY][pickX].isMine = true;
            placedMines++;
        }
    }

    for (int i = 0; i < height; i++) // funkcja zliczająca miny wokol pola
    {
        for (int j = 0; j < width; j++)
        {
            if (!playField[i][j].isMine)
            {
                for (int k = -1; k <= 1; k++) // petla sprawdzająca pola wokol aktualnie sprawdzanego
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width) // zabezpieczenie przed wyjsciem poza zakres tablicy
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

void revealMines(int width, int height, tile **playField) // funkcja wyswietlająca planszę z odkrytymi minami - po przegraniu
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
                printf(" □ ");
            }
            else if (playField[i][j].isRevealed && playField[i][j].minesAround != 0)
            {
                printf(" %d ", playField[i][j].minesAround);
            }
            else if (!playField[i][j].isRevealed)
            {
                printf(" ■ ");
            }
        }
        printf("\n");
    }
}

int checkGameStatus(int width, int height, int mines, tile **playField) // sprawdzanie aktualnego statusu gry
{
    int tilesRevealed = 0;
    for (int i = 0; i < height; i++) // przechodzi po wszystkich polach
    {
        for (int j = 0; j < width; j++)
        {
            if (playField[i][j].isRevealed) // zlicza odkryte pola
                tilesRevealed++;

            if (playField[i][j].isRevealed && playField[i][j].isMine) // odkryto mine - rozgrywka sie konczy
                return 2;                                             // odkryto mine - przegrana
        }
    }
    if (tilesRevealed == width * height - mines) // odkryto wszystkie pola poza minami - wygrana
    {
        return 1; // wygrana
    }
    if (tilesRevealed < width * height - mines) // nie odkryto jeszcze wszystkich pol - rozgrywka jest kontynuowana
    {
        return 0; // gra sie nie skonczyla - kontynuuj
    }
}

void revealEmptyTiles(int width, int height, tile **playField, int x, int y) // rekurencyjne odkrywanie pustych pol
{
    // sprawdza wszystkie pola wokol wybranego w funkcji
    if (!playField[y][x].isMine && playField[y][x].minesAround == 0) //odkrywamy rekurencyjnie pola tylko w rpzypadku gdy pole nie ma zadnej wartosci, tj nie jest miną i nie ma min wokol
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i + y >= 0 && i + y < height && j + x >= 0 && j + x < width && !playField[y + i][x + j].isMine && !playField[y + i][x + j].isRevealed)
                {
                    if (playField[y + i][x + j].minesAround > 0) // jezeli pole jest oznaczone cyfra, tj ma wokol miny to tylko je odkrywa
                    {
                        playField[y + i][x + j].isRevealed = true;
                    }
                    else // jezeli pole jest puste to je odkrywa i wywoluje na nim funkcje
                    {
                        playField[y + i][x + j].isRevealed = true;
                        revealEmptyTiles(width, height, playField, x + j, y + i);
                    }
                }
            }
        }
    }
}

int score(int width, int height, tile **playField, int multiplier) // funkcja obliczająca wynik gracza na podstawie mnożnika trudnosci i ilosci odkrytych pol
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

    // usuniecie znaku nowej linii z podanego nicku
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

    // Sortowanie wyników
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
        printf("Nie udało się otworzyć pliku z planszą.\n");
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
            if (isMine == 1)
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
