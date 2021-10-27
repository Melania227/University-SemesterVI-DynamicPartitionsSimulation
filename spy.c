#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"
#include "./sources/MemoryInfo.c"

int main(){    
    
    //attach shared memories 
    int *memoryBlock = (int*)attachMemoryBlock(FILENAME, 0);
    Process *processesBlock = (Process*)attachMemoryBlock(FILENAME, 1);
    pthread_mutex_t *mutexesBlock = (pthread_mutex_t*)attachMemoryBlock(FILENAME, 2);
    MemoryInfo *memoryInfoBlock = (MemoryInfo*)attachMemoryBlock(FILENAME, 3);
  
    if (memoryBlock == NULL || processesBlock == NULL || mutexesBlock == NULL || memoryInfoBlock == NULL)
    {
        printf("Error. Shared memory could´nt be attached.\n");
        return IPC_RESULT_ERROR;
    }
    
    //produce processes
    printf("Spy working...\n");
    printf("La memoria actual es:\n");
    pthread_mutex_lock(&mutexesBlock[0]);
    for (int i = 0; i < memoryInfoBlock->memorySize; i++)
    {
        printf("Fila %d esta ocupada por el proceso \t%d\n",i,memoryBlock[i]);
    }
    pthread_mutex_unlock(&mutexesBlock[0]);
    
    pthread_mutex_lock(&mutexesBlock[1]);
    for (int i = 0; i < memoryInfoBlock->processesArraySize; i++)
    {
        if (processesBlock[i].pId != 0)
        {
            printf("Proceso %d esta en el estado %d\n",processesBlock[i].pId,processesBlock[i].state);
        }
    }
    pthread_mutex_unlock(&mutexesBlock[1]);
    
    
    //detach shared memories 
    detachMemoryBlock((void*)memoryBlock);
    detachMemoryBlock((void*)processesBlock);
    detachMemoryBlock((void*)mutexesBlock);
    detachMemoryBlock((void*)memoryInfoBlock);

    return 0;
}