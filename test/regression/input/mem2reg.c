int fun_call() { return 0; }

int mem2reg_basic() {
    int a = 1;
    int b = 2;
    int c = 3;

    int add = a + b;

    int sub = c - a;

    int mul = add * sub;

    int bit_and = add & 1;

    int call = fun_call();

    return bit_and;
}

int mem2reg_branching() {
    int res = 0;
    if (res) {
        res = 1;
        if (res) {
            res = 2;
        } else {
            res = 3;
        }
        res = 4;
    } else {
        res = 5;
    }
    res = 6;
    return res;
}

int mem2reg_while() {
    int mem = 123;
    int to = 456;
    int reg = 789;
    while (to) {
        int reg = 432;
        mem = mem + reg;
        to = to - 2;
    }
    if (to)
        return to;
    return 987654321;
}

int mem2reg_negative() {
    int a = 6 + 7;
    return a;
}
