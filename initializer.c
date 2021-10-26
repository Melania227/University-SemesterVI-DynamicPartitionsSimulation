#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"
#include "./sources/MemoryInfo.c"

bool createSharedMemories(int memoryBlockSize, int processesBlockSize, int mutexesBlockSize){

    //if file doesnt exist create it
    //create shared memories 
    int memoryBlock = createSharedBlock(FILENAME, 0, memoryBlockSize);
    int processesBlock = createSharedBlock(FILENAME, 1, processesBlockSize);
    int mutexesBlock = createSharedBlock(FILENAME, 2, mutexesBlockSize);
    int memoryInfoBlock = createSharedBlock(FILENAME, 3, sizeof(MemoryInfo));


    bool error =   memoryBlock == IPC_RESULT_ERROR
                || processesBlock == IPC_RESULT_ERROR
                || mutexesBlock == IPC_RESULT_ERROR
                || memoryInfoBlock == IPC_RESULT_ERROR;

    return !error;

}

int main(){

    //read lines amount
    int lines;
    printf("Enter lines amount: ");
    scanf("%d", &lines); 

    //define memories size
    int memoryBlockSize =  lines*sizeof(int);
    int processesBlockSize = lines*sizeof(Process)*PROCESS_FACTOR;
    int mutexesBlockSize =  sizeof(pthread_mutex_t)*MUTEXES_AMOUNT;
  
    //create shared memories 
    if (!createSharedMemories(memoryBlockSize, processesBlockSize, mutexesBlockSize))
    {
        printf("Error. Shared memory could´nt be created.\n");
        return IPC_RESULT_ERROR;
    }

    //attach memories
    pthread_mutex_t *mutexesBlock = (pthread_mutex_t*)attachMemoryBlock(FILENAME, 2);  
    MemoryInfo *memoryInfoBlock  = (MemoryInfo*)attachMemoryBlock(FILENAME, 3);  

    if (mutexesBlock ==  NULL)
    {
        printf("Error. Shared memory could´nt be attached.\n");
        return IPC_RESULT_ERROR;
    }

    //mutexesBlock intialization
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
    //mutex intializations
    for (int i = 0; i < MUTEXES_AMOUNT; i++)
    {
        int x = pthread_mutex_init(&mutexesBlock[i], &mutexattr);
    }

    //MemoryInfo  intialization
    memcpy(memoryInfoBlock, newMemoryInfo(lines, lines*PROCESS_FACTOR), sizeof(MemoryInfo));

    //detach memories
    detachMemoryBlock((void*)mutexesBlock);
    detachMemoryBlock((void*)memoryInfoBlock);
   
    printf("Shared memory blocks created.\n");

    return 0;
}