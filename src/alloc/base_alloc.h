#ifndef _BASE_ALLOC_H_
#define _BASE_ALLOC_H_

#include <stddef.h>


typedef struct Alloc Alloc;


typedef struct {
    void*(*new)(struct Alloc*, size_t);
    void (*delete)(struct Alloc*, void *);
    void (*reset)(struct Alloc*);
    void (*finalize)(struct Alloc*);
}AllocVTab;


struct Alloc {
    AllocVTab * vtab;
};


#define new(T, size) (T)->vtab->new((T), (size))


#define delete(T, ptr) (T)->vtab->delete((T), (ptr))


#define reset(T) (T)->vtab->delete((T))


#define finalize(T) if((T) != NULL && (T)->vtab->finalize != NULL) (T)->vtab->finalize((T))


#endif



