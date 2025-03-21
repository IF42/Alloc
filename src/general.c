#include "alloc/general.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void * general_alloc_new(GeneralAlloc * self, size_t memsize) {
    GeneralAlloc_Node * node = malloc(sizeof(GeneralAlloc_Node) + memsize);

    node->head = NULL;
    node->tail = self->back;
    node->memsize = memsize;

    if(self->back == NULL) {
        self->front = node;
    } else {
        self->back->tail = node;
    }

    self->back = node;

    return node->mem;
}


static void general_alloc_delete(GeneralAlloc * self, void * mem) {
    GeneralAlloc_Node * node = (GeneralAlloc_Node*) (((char*) mem) - sizeof(GeneralAlloc_Node));
    
    if(node->tail != NULL) {
        node->tail->head = node->head;
    } else {
        self->front = node->head;
    }

    if(node->head != NULL) {
        node->head->tail = node->tail;
    } else {
        self->back = node->tail;
    }

    free(node);
}


static void * general_alloc_resize(GeneralAlloc * self, void * mem, size_t size) {
    void * resize_mem = general_alloc_new(self, size);

    if(mem != NULL) {
        GeneralAlloc_Node * node = (GeneralAlloc_Node*) (((char*) mem) - sizeof(GeneralAlloc_Node));
        memcpy(resize_mem, node->mem, node->memsize);

        if(node->tail != NULL) {
            node->tail->head = node->head;
        } else {
            self->front = node->head;
        }

        if(node->head != NULL) {
            node->head->tail = node->tail;
        } else {
            self->back = node->tail;
        }

        free(node);
    }

    return resize_mem;
}


static void general_alloc_reset(GeneralAlloc * self) {
    for(GeneralAlloc_Node * head = self->front; head != NULL;) {
         printf("here\n");
         GeneralAlloc_Node * node = head->tail;
         free(head);
         head = node;
    }
}


static void general_alloc_finalize(GeneralAlloc * self) {
    general_alloc_reset(self);
}


GeneralAlloc general_alloc(void) {
    static Alloc_VTab vtab = {
        .new = (void*(*)(Alloc*, size_t)) general_alloc_new
        , .delete = (void(*)(Alloc*, void*)) general_alloc_delete
        , .resize = (void*(*)(Alloc*, void*, size_t)) general_alloc_resize
        , .reset = (void(*)(Alloc*)) general_alloc_reset
        , .finalize = (void(*)(Alloc*)) general_alloc_finalize
    };

    return (GeneralAlloc) {
        .alloc.vtab = &vtab
    };
}




