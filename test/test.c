#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/alloc/alloc.h"
#include "../src/alloc/general.h"
#include "../src/alloc/arena.h"


void general_alloc_test(void) {
    GeneralAlloc alloc = general_alloc();

    int * value = new(ALLOC(&alloc), sizeof(int));
    *value = 42;
    printf("%d\n", *value);

    size_t * next_value = new(ALLOC(&alloc), sizeof(size_t));
    *next_value = 27;
    printf("%ld\n", *next_value);
    next_value = resize(ALLOC(&alloc), next_value, sizeof(size_t) * 2);

    printf("%ld\n", *next_value);

    *next_value = 33;
    printf("%ld\n", *next_value);
    delete(ALLOC(&alloc), value);
    delete(ALLOC(&alloc), next_value);

    finalize(ALLOC(&alloc));
}


void arena_alloc_test(void) {
    ArenaAlloc alloc = arena_alloc(1024);

    int * value = new(ALLOC(&alloc), sizeof(int));
    assert(value != NULL);
    *value = 42;
    printf("%d %ld\n", *value, *(((size_t*) value) - 1));
 
    //size_t * next = new(ALLOC(&alloc), sizeof(size_t));    

    value = resize(ALLOC(&alloc), value, sizeof(int)*2);
    printf("%d %ld\n", *value, *(((size_t*) value) - 1));

    finalize(ALLOC(&alloc));
}


int main(void) {
    //general_alloc_test();
    arena_alloc_test();
    printf("Program exit..\n");
    return EXIT_SUCCESS;
}



