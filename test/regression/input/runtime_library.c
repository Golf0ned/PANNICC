#include "../../../include/runtime.h"

int main() {
    int *foo = alloc(4);
    *foo = 67;
    print(*foo);
    dealloc(foo);

    return 0;
}
