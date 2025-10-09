int constant_fold() {
    int i = 1;
    int j;
    j = i + 13;
    int k;
    k = i + i;
    int l;
    l = j * k;
    l = l + l;
    int m;
    m = l - i;
    return m;
}

int constant_fold_overflow() {
    int i = 2147483647;
    int j;
    j = i + 1;
    return j;
}

int constant_fold_underflow() {
    int i = -2147483648;
    int j;
    j = i - 1;
    return j;
}
