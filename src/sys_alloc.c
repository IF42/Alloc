#include "alloc/sys_alloc.h"

#include <stdlib.h>
#include <stdio.h>

static void * _new(SysAlloc * self, size_t memsize) {
    MemNode * node = malloc(sizeof(MemNode) + memsize);
    
    node->next = NULL;
    node->prev = self->back;

    if(self->back != NULL) {
        self->back->next = node;
    }

    self->back = node;

    if(self->front == NULL) {
        self->front = node;
    }

    return (void*) (node + 1);
}


static void _delete(SysAlloc * self, void * mem) {
    MemNode * node = (MemNode *) mem - 1;
    
    if(node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        self->front = node->next;
    }

    if(node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        self->back = node->prev;
    }

    free(node);
}


static void _reset(SysAlloc * self) {
    while(self->front != NULL) {
        MemNode * node = self->front->next;
        free(self->front);
        self->front = node;
    }

    self->front = NULL;
    self->back = NULL;
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
    };

    return (Alloc*) self;
}



