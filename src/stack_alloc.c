#include "alloc/stack_alloc.h"

#include <stdlib.h>


static void * _new(StackAlloc * self, size_t memsize) {
    void * mem = self->head;
    self->head += memsize; 
    return mem;
} 


static void _delete(StackAlloc * self, void * mem) {
    self->head = mem;
}


static void _reset(StackAlloc * self) {
    self->head = (char*) (self + 1);
}


Alloc * stack_alloc_new(size_t memsize) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .delete = (void(*)(Alloc*, void*)) _delete
        , .reset = (void(*)(Alloc*)) _reset
        , .finalize = (void(*)(Alloc*)) free
    };

    StackAlloc * self = malloc(sizeof(StackAlloc) + memsize);

    *self = (StackAlloc) {
        .alloc.vtab = &vtab
        , .head = (char*) (self+1)
    };

    return (Alloc*) self;    
}


Alloc * stack_alloc_new_from_buffer(void * buffer) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .reset = (void(*)(Alloc*)) _reset
        , .delete = (void(*)(Alloc*, void*)) _delete
    };

    StackAlloc * self = (StackAlloc*) buffer;

    *self = (StackAlloc) {
        .alloc.vtab = &vtab
        , .head = (char*) (self+1)
    };

    return (Alloc*) self;
}







