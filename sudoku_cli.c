#include "sudoku.h"
#include "sudoku_hint.h"

int promptDifficulty(void) {
    int diff;
    printf("Select difficulty (1 = Easy, 2 = Medium, 3 = Hard, 4 = Brutal): ");
    scanf("%d", &diff);
    getchar(); // consume newline
    return diff;
}

void displayBoard(void) {
    printf("      1  2  3   4  5  6   7  8  9\n");
    const char *line = "    +---------+---------+---------+\n";
    printf("%s", line);

    for (int i = 0; i < SIZE; i++) {
        printf("  %c |", 'A' + i);
        for (int j = 0; j < SIZE; j++) {
            if (initialBoard[i][j] != 0) {
                // Initial clue: cyan.
                printf(COLOR_CYAN " %d " COLOR_RESET, workingBoard[i][j]);
            } else {
                if (workingBoard[i][j] == 0) {
                    // Empty cell.
                    printf(" . ");
                } else {
                    // User move exists.
                    if (workingBoard[i][j] == solvedBoard[i][j]) {
                        // Correct move.
                        if (solverChanges[i][j] == 1)
                            printf(COLOR_GREEN " %d " COLOR_RESET, workingBoard[i][j]);
                        else
                            printf(COLOR_WHITE " %d " COLOR_RESET, workingBoard[i][j]);
                    } else {
                        // Incorrect move.
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

int saveGame(void) {
    FILE *fp = fopen(SAVE_FILENAME, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file %s: %s\n", SAVE_FILENAME, strerror(errno));
        return -1;
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(fp, "%d ", workingBoard[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Game saved to %s\n", SAVE_FILENAME);
    return 0;
}

int loadGame(void) {
    FILE *fp = fopen(SAVE_FILENAME, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file %s: %s\n", SAVE_FILENAME, strerror(errno));
        return -1;
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(fp, "%d", &workingBoard[i][j]);
        }
    }
    fclose(fp);
    return 0;
}

int isSolved(void) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (workingBoard[i][j] == 0)
                return 0;
    return 1;
}
