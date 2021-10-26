#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct MemoryInfo MemoryInfo;

struct MemoryInfo {
    int memorySize;
    int processesArraySize;
    int currentProcess;  
};

MemoryInfo* newMemoryInfo(int memorySize, int processesArraySize);

#endif