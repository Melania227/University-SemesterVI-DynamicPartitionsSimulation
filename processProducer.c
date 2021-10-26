#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"

int firstFit(int* memory, int size, int amount){
    // retorna la posicion en que la metio o -1 si no pudo
    int init = -1;
    int end = -1;
    for (int i = 0; i < size; i++) {
        if(memory[i]==0){
            if (init==-1){
                init = i;
            }
            end = i;
        }
        else{
            init = -1;
            end = 0;
        }
        if ((end-init)+1==amount){
            printf("Espacio disponible en: %d \n", init);
            return init;
        }
    }
    return -1;
}

int bestFit(int* memory, int size, int amount){
    // retorna la posicion en que la metio o -1 si no pudo
    int initTemp = -1;
    int init = -1;
    int endTemp = -1;
    int blockSize = size;
    for (int i = 0; i < size; i++) {
        if(memory[i]==0){
            if (initTemp==-1){
                initTemp = i;
            }
            endTemp = i;
        }
        else{
            initTemp = -1;
            endTemp = 0;
            continue;
        }
        if ((endTemp-initTemp)+1>=amount){
            if((endTemp-initTemp)+1<blockSize){
                blockSize=(endTemp-initTemp)+1;
                init = initTemp;
            }
        }
    }
    printf("Espacio disponible en: %d \n", init);
    return init;
}


int main(){    
    
    //attach shared memories 
    int *memoryBlock = (int*)attachMemoryBlock(FILENAME, 0);
    Process *processesBlock = (Process*)attachMemoryBlock(FILENAME, 1);
    pthread_mutex_t *mutexesBlock = (pthread_mutex_t*)attachMemoryBlock(FILENAME, 2);
  
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
    detachMemoryBlock((void*)mutexesBlock);

    return 0;
}