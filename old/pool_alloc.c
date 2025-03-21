#include "alloc/pool_alloc.h"
#include <stdlib.h>


static void * _new(PoolAlloc * self, size_t memsize) {
    for(size_t i = 0; i < self->capacity; i++) {
        for(size_t j = 0; j < memsize; j++) {
            if(self->alloc_tab[i + j] == true) {
                break;
            }
        }
    }

    return NULL;
} 


static void _delete(PoolAlloc * self, void * mem) {
    for(size_t i = 0; i < self->capacity; i++) {
        if((self->block_list + self->block_size) == mem) {
            self->alloc_tab[i] = false;
            break;
        }
    }
}


static void _reset(StackAlloc * self) {
    memset(self->alloc_tab, 0, sizeof(bool) * self->capacity);
}


static void _finalize(SysAlloc * self) {
    if(self->block_list != NULL) {
        free(self->block_list);
    }

    if(self->alloc_tab != NULL) {
        free(self->alloc_tab);
    }
}


PoolAlloc pool_alloc_init(size_t capacity, size_t block_size) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .reset = (void(*)(Alloc*)) _reset
        , .delete = (void(*)(Alloc*, void*)) _delete
        , .finalize = (void(*)(Alloc*)) _finalize
    };

    return (PoolAlloc) {
        .alloc.vtab = &vtab
        , .capacity = capacity
        , .front = buffer
        , .end =  buffer
    };
}






