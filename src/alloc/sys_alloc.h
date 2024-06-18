#ifndef _SYS_ALLOC_H_
#define _SYS_ALLOC_H_


#include "base_alloc.h"


typedef struct MemNode {
    struct MemNode * next;
}MemNode;


typedef struct {
    Alloc alloc;
    MemNode * mem_node;
}SysAlloc;


Alloc * sys_alloc_new(void);



#endif
