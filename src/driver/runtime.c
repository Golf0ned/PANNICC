#include <stdio.h>
#include <stdlib.h>

#include "driver/runtime.h"

void *alloc(int size) { return malloc(size); }

int dealloc(void *ptr) {
    free(ptr);
    return 0;
}

int print(int value) {
    printf("%d", value);
    return 0;
}
