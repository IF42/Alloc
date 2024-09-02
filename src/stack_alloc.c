#include "alloc/stack_alloc.h"

#include <stdlib.h>


static void * _new(StackAlloc * self, size_t memsize) {
    if((size_t) (self->end - self->front) < self->capacity) {
        void * mem = self->end;
        self->end += memsize; 
        return mem;
    } else {
        return NULL;
    }
} 


static void _delete(StackAlloc * self, void * mem) {
    self->end = mem;
}


static void _reset(StackAlloc * self) {
    self->end = self->front;
}


Alloc * stack_alloc_new(size_t capacity) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .delete = (void(*)(Alloc*, void*)) _delete
        , .reset = (void(*)(Alloc*)) _reset
        , .finalize = (void(*)(Alloc*)) free
    };

    StackAlloc * self = malloc(sizeof(StackAlloc) + capacity);
    char * mem = (char *) (self + 1);

    *self = (StackAlloc) {
        .alloc.vtab = &vtab
        , .capacity = capacity
        , .front = mem
        , .end = mem
    };

    return (Alloc*) self;    
}


Alloc * stack_alloc_new_from_buffer(size_t capacity, void * buffer) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .reset = (void(*)(Alloc*)) _reset
        , .delete = (void(*)(Alloc*, void*)) _delete
    };

    StackAlloc * self = (StackAlloc*) buffer;
    char * mem = (char*) (self + 1);

    *self = (StackAlloc) {
        .alloc.vtab = &vtab
        , .capacity = capacity - sizeof(StackAlloc)
        , .front = mem
        , .end = mem
    };

    return (Alloc*) self;
}







