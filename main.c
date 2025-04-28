#include "sudoku.h"
#include "save_load.h"

int main(void) {
    srand(time(NULL));

    // Initialize solverChanges to 0.
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            solverChanges[i][j] = 0;

    int isNewGame = startGame();

    if (isNewGame) {
        int difficulty = promptDifficulty();

        // 1. Generate puzzle into workingBoard.
        generatePuzzle(workingBoard, difficulty);
        // 2. Copy puzzle to initialBoard.
        copyBoard(workingBoard, initialBoard);
        // 3. Compute solvedBoard from initialBoard.
        copyBoard(initialBoard, solvedBoard);
        if (!solveSudoku(solvedBoard)) {
            fprintf(stderr, "Error: Could not solve the puzzle.\n");
            exit(1);
        }
        printf("Generated a %s puzzle.\n", getDifficultyName(difficulty));
    } else {
        // displayBoard();
        clearInputBuffer();
    }


    char input[100];
    char command[20];
    int row, col, value;
    int running = 1;

    while (running) {
        displayBoard();
        printf("Enter a move (e.g., A5:9) or command check(c), hint(h), resolve(r), save(s), load(l), submit, quit(q): ");
        fgets(input, sizeof(input), stdin);

        trimInput(input);

        // Input validation
        if (input[0] == '\n' || strlen(input) >= sizeof(input) - 1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }

        // Analyse input
        if (!parseInput(input, &row, &col, &value, command)) {
            printf("Invalid input format. Please use the format e.g., A5:9 or a command.\n");
            continue;
        }

        // Validate cell coordinates
        if (strcmp(command, "move") == 0) {
            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
                printf("Invalid cell coordinates. Please use A to I and 1 to 9.\n");
                continue;
            }

            // Validate value
            if (value < 1 || value > 9) {
                printf("Invalid value. Please use numbers from 1 to 9.\n");
                continue;
            }

            // Move validation
            if (validateMove(row, col, value)) {
                updateBoardCell(row, col, value);
            } else {
                printf("Invalid move.\n");
            }
        } else if (strcmp(command, "check") == 0) {
            if (checkBoard())
                printf("Errors found.\n");
            else
                printf("No errors found.\n");
        } else if (strcmp(command, "hint") == 0) {
            int hr, hc, hv;
            char rule[50];  // Buffer to hold the rule string.
            if (provideHint(&hr, &hc, &hv, rule))
                printf("Hint: %c%d = %d (%s)\n", 'A' + hr, hc + 1, hv, rule);
            else
                printf("No hint available.\n");
        } else if (strcmp(command, "resolve") == 0) {
            // For each non-clue cell, if it is empty or incorrect, fill with solvedBoard value.
            for (int r = 0; r < SIZE; r++) {
                for (int c = 0; c < SIZE; c++) {
                    if (initialBoard[r][c] == 0) {
                        if (workingBoard[r][c] == 0 || workingBoard[r][c] != solvedBoard[r][c]) {
                            workingBoard[r][c] = solvedBoard[r][c];
                            solverChanges[r][c] = 1;
                        }
                    }
                }
            }
            printf("Puzzle resolved by system.\n");
            displayBoard();
            break;
        } else if (strcmp(command, "save") == 0) {
            if (saveGame() == 0)
                printf("Game saved.\n");
        } else if (strcmp(command, "load") == 0) {
            if (loadGame() == 0)
                printf("Game loaded.\n");
        } else if (strcmp(command, "submit") == 0) {
            if (isSolved())
                printf("Congratulations! You solved the puzzle.\n");
            else
                printf("Puzzle is not yet solved.\n");
        } else if (strcmp(command, "quit") == 0) {
            running = 0;
        } else {
            printf("Unknown command. Please try again.\n");
        }
    }

    printf("Thanks for playing!\n");
    return 0;
}