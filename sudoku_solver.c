#include "sudoku.h"

// Define global variables (only here, not in the header)
Board workingBoard;
Board initialBoard;
Board solvedBoard;
Board solverChanges;
const char *difficultyNames[] = { "Easy", "Medium", "Hard", "Brutal" };

const char* getDifficultyName(int difficulty) {
    if (difficulty < 1 || difficulty > 4)
        return "Unknown";
    return difficultyNames[difficulty - 1];
}

int isSafe(const Board board, int row, int col, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (board[row][i] == num) return 0;
        if (board[i][col] == num) return 0;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[startRow + r][startCol + c] == num)
                return 0;
    return 1;
}

void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
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

void removeCells(Board board, int cluesToKeep) {
    int totalCells = SIZE * SIZE;
    int cellsToRemove = totalCells - cluesToKeep;
    while (cellsToRemove > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (board[row][col] != 0) {
            int backup = board[row][col];
            board[row][col] = 0;
            Board temp;
            copyBoard(board, temp);
            int solCount = countSolutions(temp);
            if (solCount != 1) {
                board[row][col] = backup;
            } else {
                cellsToRemove--;
            }
        }
    }
}

void generatePuzzle(Board board, int difficulty) {
    // Clear board.
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;
    
    if (!generateCompleteBoard(board)) {
        fprintf(stderr, "Error generating complete board.\n");
        exit(1);
    }
    
    int cluesToKeep;
    switch (difficulty) {
        case 1: cluesToKeep = 49; break;
        case 2: cluesToKeep = 35; break;
        case 3: cluesToKeep = 31; break;
        default: cluesToKeep = 24; break;
    }
    removeCells(board, cluesToKeep);
}

int solveSudoku(Board brd) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (brd[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (isSafe(brd, row, col, num)) {
                        brd[row][col] = num;
                        if (solveSudoku(brd))
                            return 1;
                        brd[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}
