#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"

int main(){
    
    //read lines amount
    int lines;
    printf("Enter lines amount: ");
    scanf("%d", &lines); 

    //create shared memories 
    int memoryBlockSize =  lines*sizeof(int);
    int memoryBlock = createSharedBlock(FILENAME, 0, memoryBlockSize);

    int processesBlockSize = lines*sizeof(Process)*3;
    int processesBlock = createSharedBlock(FILENAME, 1, processesBlockSize);

    int mutexesBlockSize =  sizeof(pthread_mutex_t)*MUTEXES_AMOUNT;
    int mutexesBlock = createSharedBlock(FILENAME, 2, mutexesBlockSize);

    if (memoryBlock == IPC_RESULT_ERROR || processesBlock == IPC_RESULT_ERROR || mutexesBlock == IPC_RESULT_ERROR)
    {
        printf("Error. Shared memory could´nt be created.\n");
        return IPC_RESULT_ERROR;
    }

    //attach mutexes block
    pthread_mutex_t *mutexes = (pthread_mutex_t*)attachMemoryBlock(FILENAME, 2);    

    if (mutexes =  NULL)
    {
        printf("Error. Shared memory could´nt be attached.\n");
        return IPC_RESULT_ERROR;
    }

    //create mutex attr
    pthread_mutexattr_t mutexattr;

    //mutex attr Initialization 
    pthread_mutexattr_init(&mutexattr);

    //modify attribute
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);

    //mutex intializations
    for (int i = 0; i < MUTEXES_AMOUNT; i++)
    {
        int x = pthread_mutex_init(&mutexes[i], &mutexattr);
    }

    detachMemoryBlock((void*)mutexes);
   
    printf("Shared memory blocks created.\n");

    return 0;
}