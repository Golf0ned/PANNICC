int address_deref(int foo) {
    int *bar = &foo;
    int **baz = &bar;
    int ***qux = &baz;
    return ***qux;
}

int *return_ptr() {
    int foo = 1;
    return &foo;
}
