int constant_fold() {
    int i = 1;
    int j = i + 13;
    int k = i + i;
    int l = j * k;
    l = l + l;
    int m = l - i;
    return m;
}

int constant_fold_overflow() {
    int overflow = 2147483647 + 1;
    return overflow;
}

int constant_fold_underflow() {
    int underflow = -2147483648 - 1;
    return underflow;
}
