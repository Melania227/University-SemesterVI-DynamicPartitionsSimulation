#include <stdio.h>
#include <stdbool.h>

#include "./sources/sharedMemory.c"

int main(){
    
    //destroy shared memories 
    bool success = destroyMemoryBlock(FILENAME, 0)
                && destroyMemoryBlock(FILENAME, 1);

    if (!success)
    {
        printf("Shared memory block could not be deleted.\n");
        return IPC_RESULT_ERROR;
    }

    printf("Shared memory block deleted.\n");    
    
    return 0;
}