#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include "./sources/sharedMemory.c"
#include "./sources/Process.c"
#include "./sources/MemoryInfo.c"

//attach shared memories 
int *memoryBlock;
Process *processesBlock;
pthread_mutex_t *mutexesBlock;
MemoryInfo *memoryInfoBlock;


void memoryState(){
    
    pthread_mutex_lock(&mutexesBlock[0]);

    printf("\nThe current memory state is:\n\n"); 
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
    printf("┃\tLine\t\t┃\tProcess\t\t┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━┫\n");
    for (int i = 0; i < memoryInfoBlock->memorySize; i++)
    {
        if (memoryBlock[i] != 0)
        {
            printf("┃\t%d\t\t┃\t%d\t\t┃\n",i+1,memoryBlock[i]);
        }
        else{
            printf("┃\t%d\t\t┃\t \t\t┃\n",i+1);            
        }
        if (i<memoryInfoBlock->memorySize-1)
        {
            printf("┣━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━┫\n");
        }
        else{
            printf("┗━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"); 
        }  
    }

    pthread_mutex_unlock(&mutexesBlock[0]);
}

void processesState(){
    pthread_mutex_lock(&mutexesBlock[1]);
    
    printf("\nThe current processes state is:\n\n");
    
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
    printf("┃     Memory Access     ┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
    for (int i = 0; i < memoryInfoBlock->processesArraySize; i++){
        if (processesBlock[i].state == 3 ){
            printf("┃\t%d\t\t┃\n",processesBlock[i].pId);
            printf("┣━━━━━━━━━━━━━━━━━━━━━━━┫\n");
        }
    }
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"); 

    printf("┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
    printf("┃        Blocked        ┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
    for (int i = 0; i < memoryInfoBlock->processesArraySize; i++){
        if (processesBlock[i].state == 1 ){
            printf("┃\t%d\t\t┃\n",processesBlock[i].pId);
            printf("┣━━━━━━━━━━━━━━━━━━━━━━━┫\n");
        }
    }
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"); 

    printf("┏━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
    printf("┃       Executing       ┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
    for (int i = 0; i < memoryInfoBlock->processesArraySize; i++){
        if (processesBlock[i].state == 2 ){
            printf("┃\t%d\t\t┃\n",processesBlock[i].pId);
            printf("┣━━━━━━━━━━━━━━━━━━━━━━━┫\n");
        }
    }
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n"); 

    pthread_mutex_unlock(&mutexesBlock[1]);
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
    
    printf("\nWelcome to Spy Program!\n\n");
    printf("Options:\n");
    printf("1. Memory state. \n");
    printf("2. Processes state. \n");
    printf("3. Exit. \n");
    
    while (true)
    {
        //read option
        int option =2;
        printf("Please enter an option: ");
        scanf("%d", &option); 
        if (option == 1)
        {
            memoryState();
        } 
        else if(option == 2){
            processesState();
        }
        else if(option == 3){
            break;
        }
        else{
            printf("Invalid option. ");
        }
    }
   
    //detach shared memories 
    detachMemoryBlock((void*)memoryBlock);
    detachMemoryBlock((void*)processesBlock);
    detachMemoryBlock((void*)mutexesBlock);
    detachMemoryBlock((void*)memoryInfoBlock);

    return 0;
}