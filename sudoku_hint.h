#ifndef SUDOKU_HINT_H
#define SUDOKU_HINT_H

#include "sudoku.h"

int findNakedSingle(int *outRow, int *outCol, int *outVal, char *rule);
int findHiddenSingle(int *outRow, int *outCol, int *outVal, char *rule);
int findLockedCandidates(int *outRow, int *outCol, int *outVal, char *rule);

#endif // SUDOKU_HINT_H
