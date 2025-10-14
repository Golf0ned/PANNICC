int fun_call() { return 42; }

int mem2reg_basic() {
    int a;
    int b;

    a = 0;
    a = 5;
    b = a;

    return b;
}

int mem2reg_branching() {
    int x;
    int y;

    x = fun_call();
    if (x - 42)
        y = 1;
    else
        y = x + 2;

    return y;
}

int mem2reg_while() {
    int x = 0;
    int y = 0;

    while (x - 10) {
        y = y + x;
        x = x + 1;
    }

    return y;
}
