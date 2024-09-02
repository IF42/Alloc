#ifndef _STACK_ALLOC_H_
#define _STACK_ALLOC_H_ 


#include "base_alloc.h"


typedef struct {
    Alloc alloc;
    size_t capacity;
    char * front;
    char * end;
} StackAlloc;


Alloc * stack_alloc_new(size_t capacity);


Alloc * stack_alloc_new_from_buffer(size_t capacity, void * buffer);



#endif




