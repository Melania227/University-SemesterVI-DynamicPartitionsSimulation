#include "../headers/Process.h"
#include <sys/mman.h>



Process* newProcess(int pId, int size){
    //new
    Process* self = (Process*)malloc(sizeof(Process));
    
    //atributes
    self->pId;
    self->pos = 0;
    self->size = size;
    self->state = "";
    
    return self;
}