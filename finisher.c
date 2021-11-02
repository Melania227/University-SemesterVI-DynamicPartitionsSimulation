#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#include "./sources/sharedMemory.c"
#include "./sources/MemoryInfo.c"
#include "./sources/binnacle.c"

MemoryInfo *memoryInfoBlock;
pthread_mutex_t *mutexesBlock;

void writeInBinnacle(char* binnacleLine){
    pthread_mutex_lock(&mutexesBlock[3]);
    writeLine(memoryInfoBlock->binnacleRoute, binnacleLine);
    pthread_mutex_unlock(&mutexesBlock[3]);
}

int main(){

    //attach shared memories 
    memoryInfoBlock = (MemoryInfo*)attachMemoryBlock(FILENAME, 3);
    mutexesBlock = (pthread_mutex_t*)attachMemoryBlock(FILENAME, 2);

    pthread_mutex_lock(&mutexesBlock[0]);
    pthread_mutex_lock(&mutexesBlock[1]);

    if(kill(memoryInfoBlock->processProducerId, SIGKILL) == 0){
        writeInBinnacle("Finisher program killed process producer.");
        printf("Finisher program killed process producer.\n");
    }

    writeInBinnacle("Finisher program deleted shared memory blocks.");
    writeInBinnacle("Binnacle closed.");

    pthread_mutex_unlock(&mutexesBlock[0]);
    pthread_mutex_unlock(&mutexesBlock[1]);
     
    //destroy shared memories 
    bool success = destroyMemoryBlock(FILENAME, 0)
                && destroyMemoryBlock(FILENAME, 1)
                && destroyMemoryBlock(FILENAME, 2)
                && destroyMemoryBlock(FILENAME, 3);

    if (!success)
    {
        printf("Shared memory block could not be deleted.\n");
        return IPC_RESULT_ERROR;
    }

    printf("Binnacle closed.\n");
    printf("Finisher program deleted shared memory blocks.\n");

    //detach shared memories 
    detachMemoryBlock((void*)memoryInfoBlock);

    return 0;
}