#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>


void generateCode(char secretCode[])
{
    int used[10] = {0};
    int i = 0;

    while (i < 4)
    {
        int digit = rand() % 10;

        // Ï êùäéêüò äåí îåêéíÜ ìå ìçäÝí.
        if (i == 0 && digit == 0)
        {
            continue;
        }

        if (used[digit] == 0)
        {
            secretCode[i] = digit + '0';
            used[digit] = 1;
            i++;
        }
    }

    secretCode[4] = '\0';
}
void resetGame(
    char secretCode[],
    int *attempts,
    char guess[],
    int *guessLength,
    bool *gameStarted,
    bool *gameWon,
    bool *gameOver,
    bool *showMessage,
    char message[],
    int *correctPosition,
    int *wrongPosition
)
{
    generateCode(secretCode);

    *attempts = 8;

    *guessLength = 0;
    guess[0] = '\0';

    *gameStarted = true;
    *gameWon = false;
    *gameOver = false;
    *showMessage = false;

    message[0] = '\0';

    *correctPosition = 0;
    *wrongPosition = 0;
}

int main(void)
{
    InitWindow(800, 450, "Code Trap");
    SetTargetFPS(60);

    srand((unsigned int)time(NULL));

    char secretCode[5];
    generateCode(secretCode);

    bool gameStarted = false;
    bool gameWon = false;
    bool gameOver = false;

    int attempts = 8;

    char guess[5] = "";
    int guessLength = 0;

    char message[100] = "";
    bool showMessage = false;

    int correctPosition = 0;
    int wrongPosition = 0;

    while (!WindowShouldClose())
    {
        bool justStarted = false;

        if (!gameStarted && IsKeyPressed(KEY_ENTER))
        {
            gameStarted = true;
            justStarted = true;
        }

        if ((gameWon || gameOver) && IsKeyPressed(KEY_ENTER))
        {
            resetGame(
                secretCode,
                &attempts,
                guess,
                &guessLength,
                &gameStarted,
                &gameWon,
                &gameOver,
                &showMessage,
                message,
                &correctPosition,
                &wrongPosition
            );

            justStarted = true;
        }

        if (gameStarted && !gameOver && !gameWon &&
            guessLength < 4)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                if (key >= '0' && key <= '9' &&
                    guessLength < 4)
                {
                    guess[guessLength] = (char)key;
                    guessLength++;
                    guess[guessLength] = '\0';

                    showMessage = false;
                }

                key = GetCharPressed();
            }
        }

        if (gameStarted && !gameOver && !gameWon &&
            IsKeyPressed(KEY_BACKSPACE) && guessLength > 0)
        {
            guessLength--;
            guess[guessLength] = '\0';

            showMessage = false;
        }

        if (gameStarted && !gameOver && !gameWon &&
            !justStarted && IsKeyPressed(KEY_ENTER))
        {
            showMessage = true;

            if (guessLength != 4)
            {
                strcpy(message, "Enter exactly 4 digits!");
            }
            else
            {
                int usedDigits[10] = {0};
                int unique = 1;

                for (int i = 0; i < 4; i++)
                {
                    int digit = guess[i] - '0';

                    if (usedDigits[digit] == 1)
                    {
                        unique = 0;
                        break;
                    }

                    usedDigits[digit] = 1;
                }

                if (!unique)
                {
                    strcpy(message,
                           "Use four different digits!");
                }
                else
                {
                    attempts--;

                    correctPosition = 0;

                    for (int i = 0; i < 4; i++)
                    {
                        if (guess[i] == secretCode[i])
                        {
                            correctPosition++;
                        }
                    }

                    wrongPosition = 0;

                    for (int i = 0; i < 4; i++)
                    {
                        if (guess[i] == secretCode[i])
                        {
                            continue;
                        }

                        for (int j = 0; j < 4; j++)
                        {
                            if (guess[i] == secretCode[j])
                            {
                                wrongPosition++;
                                break;
                            }
                        }
                    }

                    if (strcmp(guess, secretCode) == 0)
                    {
                        gameWon = true;

                        strcpy(message,
                               "Correct! You cracked the code!");
                    }
                    else if (attempts == 0)
                    {
                        gameOver = true;

                        strcpy(message,
                               "Game over! No attempts left.");
                    }
                    else
                    {
                        strcpy(message, "Wrong code! Try again.");
                    }
                    if (!gameWon && !gameOver)
                    {
                        guessLength = 0;
                        guess[0] = '\0';
                    }
                }
            }
        }

        BeginDrawing();

        ClearBackground((Color){20, 24, 45, 255});

        DrawText("Code Trap", 300, 35, 40, GOLD);

        if (!gameStarted)
        {
            DrawText("Guess the 4-digit secret code.",
                     230, 125, 20, RAYWHITE);

            DrawText("Use four different digits.",
                     255, 165, 20, LIGHTGRAY);

            DrawText("You have 8 attempts.",
                     300, 205, 20, LIGHTGRAY);

            DrawText("Press ENTER to start",
                     290, 265, 20, GOLD);

            DrawText("Press ESC to exit",
                     315, 350, 18, GRAY);
        }
        else
        {
            DrawText("Enter your 4-digit guess:",
                     240, 100, 20, RAYWHITE);

            DrawText(guess, 350, 140, 30, GOLD);

            DrawText(
                TextFormat("Attempts left: %d", attempts),
                300, 190, 20, LIGHTGRAY
            );

            if (showMessage)
            {
                DrawText(message, 100, 230, 20, YELLOW);
                if (strcmp(message, "Wrong code! Try again.") == 0 ||
                    strcmp(message, "Correct! You cracked the code!") == 0 ||
                    strcmp(message, "Game over! No attempts left.") == 0)
                {
                    DrawText(
                        TextFormat("Correct position: %d",
                                   correctPosition),
                        230, 270, 18, GREEN
                    );

                    DrawText(
                        TextFormat("Wrong position: %d",
                                   wrongPosition),
                        230, 300, 18, SKYBLUE
                    );
                }
            }

            if (gameWon)
            {
                DrawText("YOU WIN!", 320, 340, 25, GREEN);

                DrawText("Press ENTER to play again",
                         245, 390, 20, GOLD);
            }
            else if (gameOver)
            {
                DrawText(
                    TextFormat("Secret code: %s", secretCode),
                    265, 340, 22, RED
                );

                DrawText("Press ENTER to try again",
                         245, 390, 20, GOLD);
            }
            else
            {
                DrawText("ENTER: submit | BACKSPACE: delete",
                         160, 390, 18, GRAY);
                         DrawText("ESC: exit", 340, 420, 18, GRAY);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
