#include "save_load.h"
#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h> // For mkdir on Windows
#define PATH_SEPARATOR "\\"
#define MKDIR(dir) _mkdir(dir)  // Windows uses _mkdir without permissions
#else
#include <unistd.h> // For Unix-based systems
#include <sys/types.h>
#define PATH_SEPARATOR "/"
#define MKDIR(dir) mkdir(dir, 0755)  // Unix/Linux uses mkdir with permissions
#endif


// Function to create directory and all its parent directories
int createDirectoryRecursive(const char *path) {
    char buffer[512];
    char *p;
    
    // Make a copy of the path
    strncpy(buffer, path, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    // Skip leading slashes
    p = buffer;
    while (*p == '/' || *p == '\\') p++;
    
    // Create each directory in the path
    for (; *p; p++) {
        if (*p == '/' || *p == '\\') {
            *p = '\0'; // Temporarily terminate the string at this position
            
            // Create the directory if it doesn't exist
            struct stat st;
            if (stat(buffer, &st) != 0) {
                if (MKDIR(buffer) != 0 && errno != EEXIST) {
                    fprintf(stderr, "Error creating directory %s: %s\n", buffer, strerror(errno));
                    return -1;
                }
            }
            
            *p = PATH_SEPARATOR[0]; // Restore the path separator
        }
    }
    
    // Create the final directory
    struct stat st;
    if (stat(buffer, &st) != 0) {
        if (MKDIR(buffer) != 0 && errno != EEXIST) {
            fprintf(stderr, "Error creating directory %s: %s\n", buffer, strerror(errno));
            return -1;
        }
    }
    
    return 0;
}


void getSavePath(char *path, size_t size) {
    const char *homeDir;

#ifdef _WIN32
    homeDir = getenv("USERPROFILE"); // Windows: User profile directory
    if (!homeDir) {
        homeDir = getenv("HOMEDRIVE");
        if (homeDir) {
            const char *homePath = getenv("HOMEPATH");
            if (homePath) {
                snprintf(path, size, "%s%s", homeDir, homePath);
                homeDir = path;
            }
        }
    }
#else
    homeDir = getenv("HOME"); // Unix: Home directory
#endif

    if (homeDir) {
        // Create a directory path that works for both Windows and Linux
#ifdef _WIN32
        snprintf(path, size, "%s%sDocuments%sSudoku_CLI", homeDir, PATH_SEPARATOR, PATH_SEPARATOR);
#else
        snprintf(path, size, "%s%s.local%sshare%sSudoku_CLI", homeDir, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);
#endif

        // Create the directory if it doesn't exist
        if (createDirectoryRecursive(path) != 0) {
            // If we can't create the recommended directory, fall back to the current directory
            fprintf(stderr, "Falling back to current directory for save file.\n");
            strcpy(path, ".");
        }

        // Append filename to the path
        size_t pathLen = strlen(path);
        if (pathLen > 0 && path[pathLen - 1] != PATH_SEPARATOR[0]) {
            strncat(path, PATH_SEPARATOR, size - pathLen - 1);
        }
        strncat(path, "savegame.txt", size - strlen(path) - 1);
    } else {
        // If we can't determine home directory, use current directory
        fprintf(stderr, "Warning: Could not determine home directory. Using current directory.\n");
        strncpy(path, "savegame.txt", size - 1);
        path[size - 1] = '\0';
    }
}

int saveGame(void) {
    char savePath[512];
    getSavePath(savePath, sizeof(savePath));

    FILE *fp = fopen(savePath, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file %s for writing: %s\n", savePath, strerror(errno));
        // Fallback to current directory
        fp = fopen("savegame.txt", "w");
        if (!fp) {
            fprintf(stderr, "Error: Could not create save file in current directory either: %s\n", strerror(errno));
            return -1;
        }
        strcpy(savePath, "savegame.txt");
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
        fprintf(stderr, "Error opening file %s for reading: %s\n", savePath, strerror(errno));
        // Try current directory as fallback
        fp = fopen("savegame.txt", "r");
        if (!fp) {
            fprintf(stderr, "Error: Could not find save file in default or current directory\n");
            return -1;
        }
        strcpy(savePath, "savegame.txt");
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