#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

static int createSharedBlock(char *filename, int pos, int size);
static int getSharedBlock(char *filename, int pos);
bool destroyMemoryBlock(char *filename, int pos);
void * attachMemoryBlock(char *filename, int pos);
bool detachMemoryBlock(void *block);

#define FILENAME "shmBlock"
#define IPC_RESULT_ERROR (-1)
#define MUTEXES_AMOUNT (3)

#endif