#include "../../../include/runtime.h"

int main() {
    print(67);

    int *foo = alloc(4);
    dealloc(foo);

    return 0;
}
