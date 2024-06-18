#include "alloc/sys_alloc.h"

#include <stdlib.h>

static void * _new(SysAlloc * self, size_t memsize) {
    MemNode * mem_node = malloc(sizeof(MemNode *) + memsize);
    
    mem_node->next = self->mem_node;
    self->mem_node = mem_node;

    return (void*) (mem_node + 1);
}


static void _delete(SysAlloc * self, void * mem) {
    if(self->mem_node != NULL ) {
        MemNode * prev = NULL;
        MemNode * mem_node = self->mem_node;
        MemNode * node = ((MemNode*) mem) - 1;

        while(mem_node != NULL && node != mem_node) {
            prev = mem_node;
            mem_node = mem_node->next;
        }

        if(prev != NULL) {
            prev->next = node->next;
        } else {
            self->mem_node = node->next;
        }

        free(node);
    }
}


static void _reset(SysAlloc * self) {
    while(self->mem_node != NULL) {
        MemNode * next = self->mem_node->next;
        free(self->mem_node);
        self->mem_node = next;
    }

    self->mem_node = NULL;
}


static void _finalize(SysAlloc * self) {
    _reset(self);
    free(self);
}


Alloc * sys_alloc_new(void) {
    static AllocVTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) _new
        , .delete = (void(*)(Alloc*, void*)) _delete
        , .reset = (void(*)(Alloc*)) _reset
        , .finalize = (void(*)(Alloc*)) _finalize
    };

    SysAlloc * self = malloc(sizeof(SysAlloc));

    *self = (SysAlloc) {
        .alloc.vtab = &vtab
        , .mem_node = NULL
    };

    return (Alloc*) self;
}
