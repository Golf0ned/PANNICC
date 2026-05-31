int deref(int *foo) {
    return *foo;
}

int *address(int foo) {
    return &foo;
}

int address_deref(int foo) {
    return *&foo;
}

int address_deref_many(int foo) {
    int *bar = &foo;
    int **baz = &bar;
    int ***qux = &baz;
    return ***qux;
}

int write_to_ptr(int *ptr, int **ptr_ptr) {
    *ptr = 42;
    *ptr += 42;
    **ptr_ptr = 67;
    return 0;
}

int *return_ptr() {
    int foo = 1;
    return &foo;
}
