#ifndef _FREE_LIST_ALLOC_H_
#define _FREE_LIST_ALLOC_H_

#include "base_alloc.h"

typedef struct FreeList {
    size_t size;
    struct FreeList * next;
}FreeList;


typedef struct {
    Alloc alloc;
    size_t memsize;
    FreeList * free_list;
}FreeListAlloc;


Alloc * free_list_alloc_new(size_t memsize);


Alloc * free_list_alloc_from_buffer(size_t memsize, void * buffer);



#endif
