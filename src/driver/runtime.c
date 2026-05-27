#include "runtime.h"

#include <stdio.h>
#include <stdlib.h>

int *alloc(int size) { return (int *)malloc(size); }

int dealloc(int *ptr) {
    free(ptr);
    return 0;
}

int print(int value) {
    printf("%d\n", value);
    return 0;
}
