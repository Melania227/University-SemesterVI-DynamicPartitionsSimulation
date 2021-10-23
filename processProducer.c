#include <stdio.h>
#include <stdbool.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"

int main(){    
    
    //attach shared memories 
    int *memoryBlock = (int*)attachMemoryBlock(FILENAME, 0);
    Process *processesBlock = (Process*)attachMemoryBlock(FILENAME, 0);
  
    if (memoryBlock == NULL || processesBlock == NULL)
    {
        printf("Error. Shared memory could´nt be attached.\n");
        return IPC_RESULT_ERROR;
    }

    //produce processes
    printf("Processes producer working...\n");

    //detach shared mories 
    detachMemoryBlock((void*)memoryBlock);
    detachMemoryBlock((void*)processesBlock);

    return 0;
}