#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stddef.h>

void getSavePath(char *path, size_t size);
int saveGame(void);
int loadGame(void);

#endif // SAVE_LOAD_H