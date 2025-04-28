#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

#define SIZE 9
typedef int Board[SIZE][SIZE];

#define SAVE_FILENAME "sudoku_cli_savegame.txt"

// ANSI color codes
#define COLOR_CYAN   "\033[36m"  // Clues (initial board)
#define COLOR_WHITE  "\033[37m"  // Correct user moves
#define COLOR_RED    "\033[31m"  // Incorrect user moves
#define COLOR_GREEN  "\033[32m"  // Solver-changed cells
#define COLOR_RESET  "\033[0m"

// Global variables (declared as extern)
extern Board workingBoard;    // Board that the user plays on
extern Board initialBoard;    // The initial puzzle clues
extern Board solvedBoard;     // The fully solved puzzle
extern Board solverChanges;   // Marks cells changed by the solver
extern const char *difficultyNames[]; // Difficulty names array

// Function prototypes from sudoku_solver.c
const char* getDifficultyName(int difficulty);
int isSafe(const Board board, int row, int col, int num);
void shuffle(int *array, int n);
int generateCompleteBoard(Board board);
void copyBoard(const Board src, Board dest);
int countSolutions(Board board);
void removeCells(Board board, int cluesToKeep);
void generatePuzzle(Board board, int difficulty);
int solveSudoku(Board board);

// Function prototypes from sudoku_cli.c
int promptDifficulty(void);
void displayBoard(void);
int startGame(void);
void trimInput(char *input);
void clearInputBuffer();
int parseInput(const char *input, int *row, int *col, int *value, char *command);
int validateMove(int row, int col, int value);
void updateBoardCell(int row, int col, int value);
int checkBoard(void);
int provideHint(int *outRow, int *outCol, int *outVal, char *rule);
int saveGame(void);
int loadGame(void);
int isSolved(void);

#endif // SUDOKU_H
