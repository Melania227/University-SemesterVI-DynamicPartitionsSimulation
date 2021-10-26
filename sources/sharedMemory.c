#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../headers/sharedMemory.h"


static int createSharedBlock(char *filename, int pos, int size){
    key_t key;

    key = ftok(filename, pos);
    if(key == IPC_RESULT_ERROR){
        return IPC_RESULT_ERROR;
    }

    //get shared block  ---- create it if it doesn´t exist
    return shmget(key, size, 0644 | IPC_CREAT);
}

static int getSharedBlock(char *filename, int pos){
    key_t key;

    key = ftok(filename, pos);
    if(key == IPC_RESULT_ERROR){
        return IPC_RESULT_ERROR;
    }

    return shmget(key, 0, 0644);
}

bool destroyMemoryBlock(char *filename, int pos){
    int sharedBlockId = getSharedBlock(filename, pos);

    if (sharedBlockId == IPC_RESULT_ERROR)
    {
        return NULL;    
    }
    return (shmctl(sharedBlockId, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

void *attachMemoryBlock(char *filename, int pos){
    int sharedBlockId = getSharedBlock(filename, pos);
    void *result;

    if(sharedBlockId == IPC_RESULT_ERROR){
        return NULL;
    }

    //map the shared block into this process´s memory and give me a pointer to it 
    result = shmat(sharedBlockId, NULL, 0);
    if(result == (void*) IPC_RESULT_ERROR){
        return NULL;
    }

    return result;
}


bool detachMemoryBlock(void *block){
    return (shmdt(block) != IPC_RESULT_ERROR);
}


