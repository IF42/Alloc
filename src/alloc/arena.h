#ifndef _ARENA_H_
#define _ARENA_H_


#include "alloc.h"


typedef struct {
    Alloc alloc;
    size_t capacity;
    size_t size;
    unsigned char * front;
} ArenaAlloc;


ArenaAlloc arena_alloc(size_t capacity);


#endif


