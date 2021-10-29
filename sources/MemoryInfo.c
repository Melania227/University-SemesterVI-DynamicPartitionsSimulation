#include "../headers/MemoryInfo.h"
#include <sys/mman.h>



MemoryInfo* newMemoryInfo(int memorySize, int processesArraySize){
    //new
    MemoryInfo* self = (MemoryInfo*)malloc(sizeof(MemoryInfo));
    
    //atributes
    self->memorySize = memorySize;
    self->processesArraySize = processesArraySize;
    self->processCounter = 0;

    return self;
}