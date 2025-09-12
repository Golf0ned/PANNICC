int64_t first() {
    return 1;
}

int64_t second() {
    int64_t res;
    res = first();
    res = res + 1;
    return res;
}

int64_t third() {
    int64_t res;
    res = second();
    res = res + 2;
    return res;
}

int64_t fourth() {
    int64_t res;
    res = third();
    res = res + 3;
    return res;
}

int64_t fifth() {
    int64_t res;
    res = fourth();
    res = res + 4;
    return res;
}

int64_t sixth() {
    int64_t res;
    res = fifth();
    res = res + 5;
    return res;
}

int64_t main() {
    int64_t res = 0;
    int64_t buf;
    buf = first();
    res = res + buf;
    buf = second();
    res = res + buf;
    buf = third();
    res = res + buf;
    buf = fourth();
    res = res + buf;
    buf = fifth();
    res = res + buf;
    buf = sixth();
    res = res + buf;
    return res;
}
