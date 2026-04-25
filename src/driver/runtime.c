#include <stdio.h>
#include <stdlib.h>

#include "runtime.h"

int *alloc(int size) { return (int *)malloc(size); }

int dealloc(int *ptr) {
    free(ptr);
    return 0;
}

int print(int value) {
    printf("%d", value);
    return 0;
}
