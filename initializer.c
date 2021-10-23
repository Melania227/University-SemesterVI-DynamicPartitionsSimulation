#include <stdio.h>
#include <stdbool.h>

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

    int processesBlockSize = lines*sizeof(Process)*2;
    int processesBlock = createSharedBlock(FILENAME, 1, processesBlockSize);
    
    if (memoryBlock == IPC_RESULT_ERROR || processesBlock == IPC_RESULT_ERROR)
    {
        printf("Error. Shared memory could´nt be created.\n");
        return IPC_RESULT_ERROR;
    }

    printf("Shared memory blocks created.\n");

    return 0;
}