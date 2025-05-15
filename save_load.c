#include "save_load.h"
#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h> // For mkdir on Windows
#define PATH_SEPARATOR "\\"
#else
#include <unistd.h> // For Unix systems
#define PATH_SEPARATOR "/"
#endif

void getSavePath(char *path, size_t size) {
    const char *homeDir;

#ifdef _WIN32
    homeDir = getenv("USERPROFILE"); // Windows: User-Profiles
#else
    homeDir = getenv("HOME"); // Unix: Home-Directory
#endif

    if (homeDir) {
        snprintf(path, size, "%s%sDocuments%sSudoku_CLI", homeDir, PATH_SEPARATOR, PATH_SEPARATOR);

        // Create directory if it doesn't exist
        struct stat st = {0};
        if (stat(path, &st) == -1) {
#ifdef _WIN32
            if (_mkdir(path) != 0) {
#else
            if (mkdir(path, 0700) != 0) {
#endif
                fprintf(stderr, "Error creating directory %s: %s\n", path, strerror(errno));
                return;
            }
            }

            // Path to savegame file
            snprintf(path, size, "%s%s%s", path, PATH_SEPARATOR, "savegame.txt");
        } else {
            fprintf(stderr, "Error: Could not determine home directory.\n");
        }
    }

int saveGame(void) {
    char savePath[512];
    getSavePath(savePath, sizeof(savePath));

    FILE *fp = fopen(savePath, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file %s: %s\n", savePath, strerror(errno));
        return -1;
    }

    // Save workingBoard
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(fp, "%d ", workingBoard[i][j]);
        }
        fprintf(fp, "\n");
    }

    // Save initialBoard
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(fp, "%d ", initialBoard[i][j]);
        }
        fprintf(fp, "\n");
    }

    // Save solvedBoard
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(fp, "%d ", solvedBoard[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Game saved to %s\n", savePath);
    return 0;
}

int loadGame(void) {
    char savePath[512];
    getSavePath(savePath, sizeof(savePath));

    FILE *fp = fopen(savePath, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file %s: %s\n", savePath, strerror(errno));
        return -1;
    }

    // Load workingBoard
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(fp, "%d", &workingBoard[i][j]) != 1) {
                fprintf(stderr, "Error reading workingBoard at row %d, column %d\n", i + 1, j + 1);
                fclose(fp);
                return -1;
            }
        }
    }

    // Load initialBoard
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(fp, "%d", &initialBoard[i][j]) != 1) {
                fprintf(stderr, "Error reading initialBoard at row %d, column %d\n", i + 1, j + 1);
                fclose(fp);
                return -1;
            }
        }
    }

    // Load solvedBoard
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(fp, "%d", &solvedBoard[i][j]) != 1) {
                fprintf(stderr, "Error reading solvedBoard at row %d, column %d\n", i + 1, j + 1);
                fclose(fp);
                return -1;
            }
        }
    }

    fclose(fp);
    printf("Game loaded from %s\n", savePath);
    return 0;
}

bool checkSavedGameExists(void) {
    FILE *file = fopen("saved_game.dat", "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}