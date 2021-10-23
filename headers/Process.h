#ifndef PROCESS_H
#define PROCESS_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct Process Process;

struct Process {
    int pId;
    int pos;
    int size;
    char* state;  
};

Process* newProcess();

#endif