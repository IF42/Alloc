#ifndef _SYS_ALLOC_H_
#define _SYS_ALLOC_H_


#include "alloc.h"


typedef struct GeneralAlloc_Node {
    struct GeneralAlloc_Node * tail;
    struct GeneralAlloc_Node * head;
    size_t memsize;
    char mem[];
} GeneralAlloc_Node;


typedef struct {
    Alloc alloc;
    size_t size;
    GeneralAlloc_Node * front;
    GeneralAlloc_Node * back;
} GeneralAlloc;


GeneralAlloc general_alloc(void);


#endif


