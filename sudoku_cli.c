#include "sudoku.h"
#include "sudoku_hint.h"

int promptDifficulty(void) {
    int diff;
    while (1) {
        printf("Select difficulty (1 = Easy, 2 = Medium, 3 = Hard, 4 = Brutal): ");
        if (scanf("%d", &diff) != 1) {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            while (getchar() != '\n');
            continue;
        }
        if (diff >= 1 && diff <= 4) {
            getchar(); // consume newline
            return diff;
        } else {
            printf("Invalid difficulty. Please enter a number between 1 and 4.\n");
        }
    }
}

int startGame(void) {
    char choice;
    while (1) {
        printf("Welcome to CLI Sudoku.\n");
        printf("New Game (n) or load previous game (l)? ");
        scanf(" %c", &choice);

        if (choice == 'n' || choice == 'N') {
            return 1; // New game
        } else if (choice == 'l' || choice == 'L') {
            if (loadGame() == 0) {
                printf("Loaded previous game successfully.\n");
                return 0; // Load game
            } else {
                printf("Failed to load the game. Please try again.\n");
            }
        } else {
            printf("Invalid choice. Please enter 'n' for a new game or 'l' to load a game.\n");
        }
    }
}

void trimInput(char *input) {
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayBoard(void) {
    printf("      1  2  3   4  5  6   7  8  9\n");
    const char *line = "    +---------+---------+---------+\n";
    printf("%s", line);

    for (int i = 0; i < SIZE; i++) {
        printf("  %c |", 'A' + i);
        for (int j = 0; j < SIZE; j++) {
            if (initialBoard[i][j] != 0) {
                // Vorgabewerte: Cyan.
                printf(COLOR_CYAN " %d " COLOR_RESET, workingBoard[i][j]);
            } else {
                if (workingBoard[i][j] == 0) {
                    // Leeres Feld: "."
                    printf(" . ");
                } else {
                    // Benutzereingabe.
                    if (workingBoard[i][j] == solvedBoard[i][j]) {
                        // Korrekte Eingabe: Weiß oder Grün.
                        if (solverChanges[i][j] == 1) {
                            printf(COLOR_GREEN " %d " COLOR_RESET, workingBoard[i][j]);
                        } else {
                            printf(COLOR_WHITE " %d " COLOR_RESET, workingBoard[i][j]);
                        }
                    } else {
                        // Falsche Eingabe: Rot.
                        printf(COLOR_RED " %d " COLOR_RESET, workingBoard[i][j]);
                    }
                }
            }
            if ((j + 1) % 3 == 0)
                printf("|");
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("%s", line);
    }
}

int parseInput(const char *input, int *row, int *col, int *value, char *command) {
    char buf[100];
    strcpy(buf, input);
    buf[strcspn(buf, "\n")] = '\0';

    if (isalpha((unsigned char)buf[0]) && isdigit((unsigned char)buf[1])) {
        *row = toupper(buf[0]) - 'A';
        *col = atoi(&buf[1]) - 1;
        char *colon = strchr(buf, ':');
        if (colon)
            *value = atoi(colon + 1);
        else
            *value = 0;
        strcpy(command, "move");
        return 1;
    } else {
        if (strcmp(buf, "check") == 0 || strcmp(buf, "c") == 0) {
            strcpy(command, "check");
            return 1;
        } else if (strcmp(buf, "hint") == 0 || strcmp(buf, "h") == 0) {
            strcpy(command, "hint");
            return 1;
        } else if (strcmp(buf, "resolve") == 0 || strcmp(buf, "r") == 0) {
            strcpy(command, "resolve");
            return 1;
        } else if (strcmp(buf, "save") == 0 || strcmp(buf, "s") == 0) {
            strcpy(command, "save");
            return 1;
        } else if (strcmp(buf, "load") == 0 || strcmp(buf, "l") == 0) {
            strcpy(command, "load");
            return 1;
        } else if (strcmp(buf, "submit") == 0) {
            strcpy(command, "submit");
            return 1;
        } else if (strcmp(buf, "quit") == 0 || strcmp(buf, "q") == 0) {
            strcpy(command, "quit");
            return 1;
        }
    }
    return 0;
}

int validateMove(int row, int col, int value) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || value < 1 || value > 9)
        return 0;
    return 1;
}

void updateBoardCell(int row, int col, int value) {
    if (initialBoard[row][col] != 0) {
        printf("Cannot overwrite an initial clue.\n");
        return;
    }
    workingBoard[row][col] = value;
}

int checkBoard(void) {
    // Stub: Add proper error-checking if desired.
    return 0;
}

int provideHint(int *outRow, int *outCol, int *outVal, char *rule) {
    if (findNakedSingle(outRow, outCol, outVal, rule))
        return 1;
    if (findHiddenSingle(outRow, outCol, outVal, rule))
        return 1;
    if (findLockedCandidates(outRow, outCol, outVal, rule))
        return 1;
    
    // Fallback: return first empty cell from solvedBoard.
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (workingBoard[r][c] == 0) {
                *outRow = r;
                *outCol = c;
                *outVal = solvedBoard[r][c];
                strcpy(rule, "Fallback");
                return 1;
            }
        }
    }
    return 0;
}

int isSolved(void) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (workingBoard[i][j] == 0)
                return 0;
    return 1;
}
