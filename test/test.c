#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/alloc/alloc.h"

#define TEST_SIZE 100
int * arr[TEST_SIZE];


void sys_alloc_test(void) {
    Alloc * alloc = sys_alloc_new();

    for(size_t i = 0; i < TEST_SIZE; i++) {
        arr[i] = new(alloc, sizeof(int));
        *arr[i] = i;
    }

    size_t sum = 0;

    for(size_t i = TEST_SIZE -1; i > 0; i--) {
        sum += *arr[i];
        delete(alloc, arr[i]);
    }

    printf("sum: %ld\n", sum);
    fflush(stdout);

    for(size_t i = 0; i < TEST_SIZE; i++) {
        arr[i] = new(alloc, sizeof(int));
        *arr[i] = i;
    }

    for(size_t i = TEST_SIZE/4; i < TEST_SIZE/2; i++) {
        delete(alloc, arr[i]);
    }

    finalize(alloc);
}


void sys_alloc_realloc_test(void) {
    size_t size = 0;
    size_t index = 0; 
    void * ptr = NULL;
    Alloc * alloc = sys_alloc_new();

    while(size < TEST_SIZE) {
        if(size == 0) {
            size = 2;
            ptr = new(alloc, size);
        } else {
            size *= 2;
            void * mem = new(alloc, size);
            delete(alloc, ptr);
            ptr = mem;
        }

        index ++;

        printf("%ld - %ld\n", index, size);
    }

    finalize(alloc);
}



int main(void) {
    //stack_alloc_test();
    //test_tensor();
    //free_list_alloc_test();
    sys_alloc_test();
    sys_alloc_realloc_test();

    printf("Program exit..\n");
    return EXIT_SUCCESS;
}



