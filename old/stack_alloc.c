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


StackAlloc stack_alloc(size_t capacity, void * buffer) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .reset = (void(*)(Alloc*)) _reset
        , .delete = (void(*)(Alloc*, void*)) _delete
    };

    return (StackAlloc) {
        .alloc.vtab = &vtab
        , .capacity = capacity
        , .front = buffer
        , .end =  buffer
    };
}







