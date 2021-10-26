#include "../headers/MemoryInfo.h"
#include <sys/mman.h>



MemoryInfo* newMemoryInfo(int memorySize, int processesArraySize){
    //new
    MemoryInfo* self = (MemoryInfo*)malloc(sizeof(MemoryInfo));
    
    //atributes
    self->currentProcess = 0;
    self->memorySize = memorySize;
    self->processesArraySize = processesArraySize;

    return self;
}