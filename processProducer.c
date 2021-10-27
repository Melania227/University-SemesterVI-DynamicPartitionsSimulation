#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"
#include "./sources/MemoryInfo.c"

int *memoryBlock;
Process *processesBlock;
pthread_mutex_t *mutexesBlock;
MemoryInfo *memoryInfoBlock;

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
           // printf("Espacio disponible en: %d \n", init);
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
  //  printf("Espacio disponible en: %d \n", init);
    return init;
}

int worstFit(int* memory, int size, int amount){
    // retorna la posicion en que la metio o -1 si no pudo
    int initTemp = -1;
    int init = -1;
    int endTemp = -1;
    int blockSize = 0;
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
        if ((endTemp-initTemp)+1>amount){
            if((endTemp-initTemp)+1>blockSize){
                blockSize=(endTemp-initTemp)+1;
                init = initTemp;
            }
        }
    }
  //  printf("Espacio disponible en: %d \n", init);
    return init;
}
 
int randomInRange(int min, int max){
    return rand() % (max + 1 - min) + min;
}

void modifyProcess(Process *process, int pId, int pos, int lines, int state){
    pthread_mutex_lock(&mutexesBlock[1]);  
    process->pId = pId;
    process->pos = pos;
    process->lines = process->lines;
    process->state = state;
    pthread_mutex_unlock(&mutexesBlock[1]); 
}

void modifyProcessPos(Process *process, int pos){
    pthread_mutex_lock(&mutexesBlock[1]);  
    process->pos = pos;
    pthread_mutex_unlock(&mutexesBlock[1]); 
}

void modifyProcessState(Process *process, int state){
    pthread_mutex_lock(&mutexesBlock[1]);  
    process->state = state;
    pthread_mutex_unlock(&mutexesBlock[1]); 
}

void* process(){
 
    int lines = randomInRange(1,10);
    int watingTime = randomInRange(20,60)*1000000;

    Process *process;

    pthread_mutex_lock(&mutexesBlock[1]);
    for (int i = 0; i < memoryInfoBlock->processesArraySize; i++)
    {
        if (processesBlock[i].pId == 0)
        {
            process = &processesBlock[i];
            break;
        }    
    }
    process->pId = pthread_self();
    process->lines = lines;
    process->state = 1;
    pthread_mutex_unlock(&mutexesBlock[1]);

    printf("Process created pid: %d\n",process->pId);
    
    pthread_mutex_lock(&mutexesBlock[0]); 
    modifyProcessState(process,3); 
    int pos = worstFit(memoryBlock,memoryInfoBlock->memorySize,lines);
    if (pos != -1)
    {
        modifyProcessPos(process,pos);
        for (int i = pos; i < pos+lines; i++)
        {
            memoryBlock[i] = process->pId;
        }  
    }
    modifyProcessState(process,2); 
    pthread_mutex_unlock(&mutexesBlock[0]); 
    if (pos == -1)
    {
        return NULL;    
    }
    
    usleep(watingTime);
    
    modifyProcessState(process,1); 
    pthread_mutex_lock(&mutexesBlock[0]); 
    modifyProcessState(process,3); 
    for (int i = pos; i < pos+lines; i++)
    {
        memoryBlock[i]=0;
    }
    modifyProcess(process,0,0,0,0);
    pthread_mutex_unlock(&mutexesBlock[0]); 

    return NULL;
}

int main(){    
    
    //attach shared memories 
    memoryBlock = (int*)attachMemoryBlock(FILENAME, 0);
    processesBlock = (Process*)attachMemoryBlock(FILENAME, 1);
    mutexesBlock = (pthread_mutex_t*)attachMemoryBlock(FILENAME, 2);
    memoryInfoBlock = (MemoryInfo*)attachMemoryBlock(FILENAME, 3);
  
    if (memoryBlock == NULL || processesBlock == NULL || mutexesBlock == NULL || memoryInfoBlock == NULL)
    {
        printf("Error. Shared memory could´nt be attached.\n");
        return IPC_RESULT_ERROR;
    }

    //produce processes
    printf("Processes producer working...\n");

    int waitingTime = randomInRange(30,60)*1000000;

    int threadsAmount = 0;
    pthread_t *threads = malloc(sizeof(pthread_t));

    while (true)
    {
        threadsAmount++;
        threads = (pthread_t*)realloc(threads,sizeof(pthread_t)*threadsAmount);
        
        //Create thread
        pthread_t t1;
        pthread_create(&t1, NULL, &process, NULL);

        threads[threadsAmount-1] = t1;       

        usleep(waitingTime);
    }

    for (int i = 0; i < threadsAmount; i++)
    {
        //Join the PathFinderExecute Thread
        pthread_join(threads[i], NULL);
    }
    
    //detach shared memories 
    detachMemoryBlock((void*)memoryBlock);
    detachMemoryBlock((void*)processesBlock);
    detachMemoryBlock((void*)mutexesBlock);
    detachMemoryBlock((void*)memoryInfoBlock);

    return 0;
}