#ifndef _STACK_ALLOC_H_
#define _STACK_ALLOC_H_ 


#include "base_alloc.h"


typedef struct {
    Alloc alloc;
    char * head;
} StackAlloc;


Alloc * stack_alloc_new(size_t memsize);


Alloc * stack_alloc_new_from_buffer(void * buffer);



#endif




