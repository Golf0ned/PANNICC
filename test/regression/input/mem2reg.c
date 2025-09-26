int64_t fun_call() {
    return 0;
}

int64_t mem2reg_basic() {
    int64_t a = 1;
    int64_t b = 2;
    int64_t c = 3;

    int64_t add;
    add = a + b;

    int64_t sub;
    sub = c - a;

    int64_t mul;
    mul = add * sub;

    int64_t bit_and;
    bit_and = add & 1;

    int64_t call;
    call = fun_call();

    return bit_and;
}

int64_t mem2reg_branching() {
    int64_t res = 0;
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

int64_t mem2reg_negative() {
    int64_t a;
    a = 6 + 7;
    return a;
}
