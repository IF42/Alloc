#include "alloc/arena.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline bool arena_alloc_is_available(ArenaAlloc * self, size_t memsize) {
    if(self->size + memsize + sizeof(size_t) < self->capacity) {
        return true;
    } else {
        return false;
    }
}


static void * arena_alloc_new(ArenaAlloc * self, size_t memsize) {
    if(arena_alloc_is_available(self, memsize) == true) {
        /*
         * allocate required memory
         */
        void * mem = &self->front[self->size];
        
        /*
         * create metadata header with information about allocated memory size
         */
        *((size_t*) mem) = memsize;

        /*
         * update end pointer
         */
        self->size += memsize + sizeof(size_t);
        //self->size = self->size + (((size_t) self->size) % 16);

        return ((char*) mem) + sizeof(size_t);
    } else {
        return NULL;
    }
} 


static inline bool arena_alloc_is_on_top(ArenaAlloc * self, void * origin) {
    if (origin == &self->front[self->size - *((size_t*) origin) - sizeof(size_t)]) {
        return true;
    } else {
        return false;
    }
}


static void arena_alloc_delete(ArenaAlloc * self, void * mem) {
    unsigned char * origin = ((unsigned char*) mem) - sizeof(size_t);
    if(arena_alloc_is_on_top(self, origin) == true) {
        self->size -= *origin;
    }
}


static void arena_alloc_reset(ArenaAlloc * self) {
    self->size = 0;
}


static void * arena_alloc_resize(ArenaAlloc * self, void * mem, size_t memsize) {
    if(mem == NULL) {
        return arena_alloc_new(self, memsize);
    } else {
        unsigned char * origin = ((unsigned char*) mem) - sizeof(size_t);
        if(*((size_t*) origin) >= memsize) {
            *((size_t*) origin) = memsize;
            return mem;
        } else if (arena_alloc_is_on_top(self, origin) == true) {
            /*
             * if the memory is on the end of the arena heap, just increase allocated memory area
             */
            if(((self->size - *((size_t*) origin)) + memsize + sizeof(size_t)) < self->capacity) {
                self->size += memsize - *((size_t*) origin);
                *((size_t*) origin) = memsize;
                return mem;
            } else {
                return NULL;
            }
        } else {     
            void * new_mem = arena_alloc_new(self, memsize);
            if(new_mem != NULL) {
                memcpy(new_mem, mem, *((size_t*) origin));
                return new_mem;
            }
        }
    }

    return NULL;
}


static void arena_alloc_finalize(ArenaAlloc * self) {
    if(self->front != NULL) {
        free(self->front);
    }
}


ArenaAlloc arena_alloc(size_t capacity) {
    static Alloc_VTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) arena_alloc_new
        , .reset = (void(*)(Alloc*)) arena_alloc_reset
        , .resize = (void*(*)(Alloc*, void*, size_t)) arena_alloc_resize
        , .delete = (void(*)(Alloc*, void*)) arena_alloc_delete
        , .finalize = (void(*)(Alloc*)) arena_alloc_finalize
    };

    void * mem = malloc(capacity);

    return (ArenaAlloc) {
        .alloc.vtab = &vtab
        , .capacity = capacity
        , .front = mem
    };
}


