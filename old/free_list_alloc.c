#include "alloc/free_list_alloc.h"

#include <stdlib.h>


static void * _new(FreeListAlloc * self, size_t memsize) {
    FreeList * current = self->free_list;
    FreeList * prev = NULL;

    while(current != NULL) {
        if(current->size >= memsize) {
            if(current->size > memsize + sizeof(FreeList)){
                FreeList * next = (FreeList*) ((char*) current) + sizeof(FreeList) + memsize;
                next->next = current->next;
                next->size = current->size - memsize - sizeof(FreeList);
                    
                current->next = next;
                current->size = memsize;
            }

            if(prev != NULL) {
                prev->next = current->next;
            } else {
                self->free_list = current->next;
            }

            return current + 1;
        } 

        prev = current;
        current = current->next;
    }

    return NULL;
}


static void _delete(FreeListAlloc * self, void * mem) {
    FreeList * block = ((FreeList*) mem) - 1;

    block->next = self->free_list;
    self->free_list = block;

    // perform coalescence
    FreeList * current = self->free_list;
    FreeList * prev = NULL;

    while(current != NULL) {
        if(prev != NULL && (FreeList*)((char*) prev) + prev->size + sizeof(FreeList) == current) {
            prev->size += current->size + sizeof(FreeList);
            prev->next = current->next;
            current = prev;
        }

        prev = current;
        current = current->next;
    }
}


static void _reset(FreeListAlloc * self) {
    FreeList * free_list = (FreeList*) (self+1);

    *free_list = (FreeList) {
        .size = self->memsize
        , .next = NULL
    };

    self->free_list = free_list;
}


Alloc * free_list_alloc_new(size_t memsize) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .delete = (void(*)(Alloc*, void*)) _delete
        , .reset = (void(*)(Alloc*)) _reset
        , .finalize = (void(*)(Alloc*)) free
    };

    FreeListAlloc * self = malloc(sizeof(FreeListAlloc) + sizeof(FreeList) + memsize);
    FreeList * free_list = (FreeList*) (self+1);
    
    *free_list = (FreeList) {
        .size = memsize
        , .next = NULL
    };

    *self = (FreeListAlloc) {
        .alloc.vtab = &vtab
        , .free_list = free_list
    };

    return (Alloc*) self;
}


Alloc * free_list_alloc_from_buffer(size_t memsize, void * buffer) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .delete = (void(*)(Alloc*, void*)) _delete
        , .reset = (void(*)(Alloc*)) _reset
    };

    FreeListAlloc * self = (FreeListAlloc*) buffer;
    FreeList * free_list = (FreeList*) (self+1);
    
    *free_list = (FreeList) {
        .size = memsize - sizeof(FreeList)
        , .next = NULL
    };

    *self = (FreeListAlloc) {
        .alloc.vtab = &vtab
        , .free_list = free_list
    };

    return (Alloc*) self;
}




