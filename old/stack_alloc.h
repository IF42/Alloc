#ifndef _STACK_ALLOC_H_
#define _STACK_ALLOC_H_ 


#include "alloc_base.h"


typedef struct {
    Alloc alloc;
    size_t capacity;
    char * front;
    char * end;
} StackAlloc;



StackAlloc stack_alloc(size_t capacity, void * buffer);



#endif




