#ifndef _BASE_ALLOC_H_
#define _BASE_ALLOC_H_

#include <stddef.h>


typedef struct Alloc Alloc;


typedef struct {
    void * (*new)(struct Alloc*, size_t);
    void (*delete)(struct Alloc*, void *);
    void * (*newa)(struct Alloc*, size_t, size_t);
    void * (*resize)(struct Alloc*, void *, size_t);
    void (*reset)(struct Alloc*);
    void (*finalize)(struct Alloc*);
}Alloc_VTab;


struct Alloc {
    Alloc_VTab * vtab;
};


#define ALLOC(T) (Alloc*)(T)


#define new(T, size) (T)->vtab->new((T), (size))


#define delete(T, ptr) (T)->vtab->delete((T), (ptr))


#define newa(T, align, size) (T)->vtab->newa((T), (align), (size))


#define resize(T, ptr, size) (T)->vtab->resize((T), (ptr), (size))


#define reset(T) (T)->vtab->delete((T))


#define finalize(T) \
    if((T) != NULL && (T)->vtab->finalize != NULL) (T)->vtab->finalize((T))


#endif



