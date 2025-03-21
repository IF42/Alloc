#ifndef _POOL_ALLOC_H_
#define _POOL_ALLOC_H_


#include "alloc_base.h"


typedef struct {
    Alloc alloc;
    size_t block_size;
    size_t capacity;
    char * block_list;
    bool * alloc_tab;
} PoolAlloc;


PoolAlloc pool_alloc_init(size_t capacity, size_t block_size);



#endif
