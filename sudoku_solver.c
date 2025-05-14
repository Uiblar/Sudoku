#include "sudoku.h"
#include <stdbool.h>

Board workingBoard;
Board initialBoard;
Board solvedBoard;
Board solverChanges;
const char *difficultyNames[] = { "Easy", "Medium", "Hard", "Brutal" };

const char* getDifficultyName(const int difficulty) {
    if (difficulty < 1 || difficulty > 4)
        return "Unknown";
    return difficultyNames[difficulty - 1];
}

int isSafe(const Board board, const int row, const int col, const int num) {
    for (int i = 0; i < SIZE; i++) {
        if (board[row][i] == num)
            return 0;
        if (board[i][col] == num)
            return 0;
    }
    const int startRow = row - row % 3;
    const int startCol = col - col % 3;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[startRow + r][startCol + c] == num)
                return 0;
    return 1;
}

void shuffle(int *array, const int n) {
    for (int i = n - 1; i > 0; i--) {
        const int j = rand() % (i + 1);
        const int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int generateCompleteBoard(Board board) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 0) {
                int candidates[SIZE];
                for (int i = 0; i < SIZE; i++) {
                    candidates[i] = i + 1;
                }
                shuffle(candidates, SIZE);
                for (int i = 0; i < SIZE; i++) {
                    if (isSafe(board, row, col, candidates[i])) {
                        board[row][col] = candidates[i];
                        if (generateCompleteBoard(board))
                            return 1;
                        board[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

void copyBoard(const Board src, Board dest) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            dest[i][j] = src[i][j];
}

void countSolutionsUtil(Board board, int *count) {
    if (*count > 1) return;
    int row = -1, col = -1, foundEmpty = 0;
    for (int i = 0; i < SIZE && !foundEmpty; i++) {
        for (int j = 0; j < SIZE && !foundEmpty; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                foundEmpty = 1;
            }
        }
    }
    if (!foundEmpty) {
        (*count)++;
        return;
    }
    for (int num = 1; num <= SIZE; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            countSolutionsUtil(board, count);
            board[row][col] = 0;
            if (*count > 1) return;
        }
    }
}

int countSolutions(Board board) {
    int count = 0;
    countSolutionsUtil(board, &count);
    return count;
}

bool removeCellsWithLimit(Board board, const int cluesToKeep) {
    const int totalCells = SIZE * SIZE;
    int cellsToRemove = totalCells - cluesToKeep;
    int iterationCount = 0;
    const int MAX_ITERATIONS = 1000;

    while (cellsToRemove > 0) {
        iterationCount++;
        if (iterationCount % 200 == 0) {
            printf("Attempting to remove cells... %d iterations so far.\n", iterationCount);
        }
        if (iterationCount > MAX_ITERATIONS) {
            return false;
        }

        const int row = rand() % SIZE;
        const int col = rand() % SIZE;
        if (board[row][col] != 0) {
            const int backup = board[row][col];
            board[row][col] = 0;
            Board temp;
            copyBoard(board, temp);
            const int solCount = countSolutions(temp);
            if (solCount != 1) {
                board[row][col] = backup;
            } else {
                cellsToRemove--;
            }
        }
    }
    return true;
}

void generatePuzzle(Board board, const int difficulty) {
    int cluesToKeep;
    switch (difficulty) {
        case 1: cluesToKeep = 49; break; // Easy
        case 2: cluesToKeep = 35; break; // Medium
        case 3: cluesToKeep = 31; break; // Hard
        default: cluesToKeep = 22; break; // Brutal
    }

    // We'll attempt multiple times to create a puzzle with the desired # of clues.
    const int MAX_RETRIES = 500;
    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        printf("Starting with attempt nr. %d to generate a puzzle\n", attempt+1);
        // 1. Clear board
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                board[i][j] = 0;

        // 2. Generate a fully solved board
        if (!generateCompleteBoard(board)) {
            fprintf(stderr, "Error generating complete board.\n");
            continue; // try again
        }

        // 3. Try to remove cells
        if (removeCellsWithLimit(board, cluesToKeep)) {
            // Success, we got a puzzle with a unique solution & # of clues
            return;
        }
        // If removeCellsWithLimit() fails, we try again
    }

    // If we exit the loop, we failed to create a puzzle after MAX_RETRIES.
    // You could handle that gracefully:
    fprintf(stderr, "Failed to create puzzle with %d clues after %d attempts.\n", (SIZE*SIZE) - cluesToKeep, MAX_RETRIES);
    exit(1);
}

int solveSudoku(Board board) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board))
                            return 1;
                        board[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}
