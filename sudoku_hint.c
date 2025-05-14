#include "sudoku_hint.h"

// find the first naked single in workingBoard if there are any
int findNakedSingle(int *outRow, int *outCol, int *outVal, char *rule) {
    // Iterate over all cells in the workingBoard (declared as global in sudoku.h)
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (workingBoard[r][c] == 0) { // Only look at empty cells.
                int possibleCount = 0;
                int candidate = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(workingBoard, r, c, num)) {
                        possibleCount++;
                        candidate = num;
                    }
                }
                if (possibleCount == 1) {
                    *outRow = r;
                    *outCol = c;
                    *outVal = candidate;
                    strcpy(rule, "NakedSingle");
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Try to find a locked candidate (pointing pair/triple) in a 3x3 box.
// For each 3x3 box, for each candidate, check if the candidate is possible
// only in one row (or column) within that box. If so, then look for an empty
// cell in that same row (outside the box) where that candidate is possible.
int findLockedCandidates(int *outRow, int *outCol, int *outVal, char *rule) {
    // Iterate over each 3x3 box.
    for (int boxRow = 0; boxRow < 3; boxRow++) {
        for (int boxCol = 0; boxCol < 3; boxCol++) {
            const int startRow = boxRow * 3;
            const int startCol = boxCol * 3;
            for (int candidate = 1; candidate <= 9; candidate++) {
                int rowCount[3] = {0, 0, 0};
                // For each cell in the box:
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) {
                        const int cellRow = startRow + r;
                        const int cellCol = startCol + c;
                        if (workingBoard[cellRow][cellCol] == 0 && isSafe(workingBoard, cellRow, cellCol, candidate)) {
                            rowCount[r]++;
                        }
                    }
                }
                // If candidate appears in only one row in this box.
                int uniqueRowIndex = -1;
                int countRows = 0;
                for (int r = 0; r < 3; r++) {
                    if (rowCount[r] > 0) {
                        uniqueRowIndex = r;
                        countRows++;
                    }
                }
                if (countRows == 1) {
                    const int targetRow = startRow + uniqueRowIndex;
                    // Find an empty cell in that row outside the current box.
                    for (int col = 0; col < SIZE; col++) {
                        if (col < startCol || col >= startCol + 3) {
                            if (workingBoard[targetRow][col] == 0 && isSafe(workingBoard, targetRow, col, candidate)) {
                                *outRow = targetRow;
                                *outCol = col;
                                *outVal = candidate;
                                strcpy(rule, "LockedCandidates");
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// Try to find a hidden single in workingBoard.
// For each empty cell, compute candidate numbers. Then for each candidate,
// check if in its row, column, or box, that candidate can appear in any other cell.
int findHiddenSingle(int *outRow, int *outCol, int *outVal, char *rule) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (workingBoard[r][c] == 0) {
                int candidates[10] = {0};
                int count = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(workingBoard, r, c, num)) {
                        candidates[num] = 1;
                        count++;
                    }
                }
                // Skip naked singles (handled by findNakedSingle)
                if (count <= 1)
                    continue;
                // Check row uniqueness.
                for (int num = 1; num <= 9; num++) {
                    if (!candidates[num])
                        continue;
                    int uniqueInRow = 1;
                    for (int cc = 0; cc < SIZE; cc++) {
                        if (cc == c) continue;
                        if (workingBoard[r][cc] == 0 && isSafe(workingBoard, r, cc, num))
                            uniqueInRow = 0;
                    }
                    if (uniqueInRow) {
                        *outRow = r;
                        *outCol = c;
                        *outVal = num;
                        strcpy(rule, "HiddenSingle");
                        return 1;
                    }
                }
                // Check column uniqueness.
                for (int num = 1; num <= 9; num++) {
                    if (!candidates[num])
                        continue;
                    int uniqueInCol = 1;
                    for (int rr = 0; rr < SIZE; rr++) {
                        if (rr == r) continue;
                        if (workingBoard[rr][c] == 0 && isSafe(workingBoard, rr, c, num))
                            uniqueInCol = 0;
                    }
                    if (uniqueInCol) {
                        *outRow = r;
                        *outCol = c;
                        *outVal = num;
                        strcpy(rule, "HiddenSingle");
                        return 1;
                    }
                }
                // Check 3x3 box uniqueness.
                const int startRow = r - r % 3;
                const int startCol = c - c % 3;
                for (int num = 1; num <= 9; num++) {
                    if (!candidates[num])
                        continue;
                    int uniqueInBox = 1;
                    for (int rr = 0; rr < 3; rr++) {
                        for (int cc = 0; cc < 3; cc++) {
                            const int cellRow = startRow + rr;
                            const int cellCol = startCol + cc;
                            if (cellRow == r && cellCol == c)
                                continue;
                            if (workingBoard[cellRow][cellCol] == 0 && isSafe(workingBoard, cellRow, cellCol, num))
                                uniqueInBox = 0;
                        }
                    }
                    if (uniqueInBox) {
                        *outRow = r;
                        *outCol = c;
                        *outVal = num;
                        strcpy(rule, "HiddenSingle");
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
