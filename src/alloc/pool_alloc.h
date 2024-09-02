#ifndef _POOL_ALLOC_H_
#define _POOL_ALLOC_H_


#include "base_alloc.h"


typedef struct {
    Alloc alloc;
    char * head;
} PoolAlloc;


Alloc * pool_alloc_new(size_t memsize);


Alloc * pool_alloc_new_from_buffer(void * buffer);



#endif
