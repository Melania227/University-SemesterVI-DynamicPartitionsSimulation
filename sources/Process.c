#include "../headers/Process.h"
#include <sys/mman.h>



Process* newProcess(int pId, int lines){
    //new
    Process* self = (Process*)malloc(sizeof(Process));
    
    //atributes
    self->pId;
    self->pos = 0;
    self->lines = lines;
    self->state = 0;
    
    return self;
}