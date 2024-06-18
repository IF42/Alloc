#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/alloc/alloc.h"


typedef struct {
    Alloc * alloc;
    size_t ndim;
    size_t * shape;
    size_t * stride;
    float * data;
} Tensor;


static Tensor * Tensor_Allocate(Alloc * alloc, size_t ndim, size_t * shape) {
    Tensor * self = new(alloc, sizeof(Tensor));

    *self = (Tensor) {
        .alloc = alloc
        , .ndim = ndim
        , .shape = new(alloc, sizeof(size_t) * ndim)
        , .stride = new(alloc, sizeof(size_t) * ndim)
    };

    memcpy(self->shape, shape, sizeof(size_t) * ndim);

    return self;
}


Tensor * Tensor_NewFromArray(size_t ndim, size_t * shape, float * data) {
    Alloc * alloc = stack_alloc_new(sizeof(Tensor) + (sizeof(size_t) * ndim * 2));
    Tensor * self = Tensor_Allocate(alloc, ndim, shape);
    self->data = data;
    return self;
}


size_t Tensor_Size(size_t ndim, size_t * shape) {
    size_t size = shape[0];

    for(size_t i = 1; i < ndim; i++) {
        return size *= shape[i];
    }

    return size;
}


Tensor * Tensor_New(size_t ndim, size_t * shape) {
    size_t size = Tensor_Size(ndim, shape);
    Alloc * alloc = stack_alloc_new(sizeof(Tensor) + (sizeof(size_t) * ndim * 2) + (sizeof(float) * size));
    Tensor * self = Tensor_Allocate(alloc, ndim, shape);
    return self;
}


void Tensor_Finalize(Tensor * self) {
    finalize(self->alloc);    
}


void stack_alloc_test(void) {
    Alloc * alloc = stack_alloc_new(sizeof(float) + sizeof(int));

    int * int_val = new(alloc, sizeof(int));
    float * float_val = new(alloc, sizeof(float));

    *int_val = 42;
    *float_val = 3.14;

    printf("%f %d\n", *float_val, *int_val);

    delete(alloc, float_val);

    finalize(alloc);
}


void test_tensor(void) {
    Tensor * t1 = Tensor_New(2, (size_t[]){2,2});
    printf("ndim: %ld, shape: [%ld %ld]\n", t1->ndim, t1->shape[0], t1->shape[1]);
    Tensor_Finalize(t1);
}


void free_list_alloc_test(void) {
    Alloc * alloc = free_list_alloc_new(sizeof(float) + sizeof(int) + 42);

    int * int_val = new(alloc, sizeof(int));
    float * float_val = new(alloc, sizeof(float));

    *int_val = 42;
    *float_val = 3.14;

    printf("%f %d\n", *float_val, *int_val);

    delete(alloc, int_val);

    int_val = new(alloc, sizeof(int));
    *int_val = 27;

    printf("%f %d\n", *float_val, *int_val);

    finalize(alloc);
}


void sys_alloc_test(void) {
    Alloc * alloc = sys_alloc_new();

    int * int_val = new(alloc, sizeof(int));
    float * float_val = new(alloc, sizeof(float));

    *int_val = 42;
    *float_val = 3.14;

    printf("%f %d\n", *float_val, *int_val);

    delete(alloc, int_val);

    int_val = new(alloc, sizeof(int));
    *int_val = 27;

    printf("%f %d\n", *float_val, *int_val);

    finalize(alloc);
}



int main(void) {
    stack_alloc_test();
    test_tensor();
    free_list_alloc_test();
    sys_alloc_test();
    
    printf("Program exit..\n");
    return EXIT_SUCCESS;
}



