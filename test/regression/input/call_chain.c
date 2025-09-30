int first() {
    return 1;
}

int second() {
    int res;
    res = first();
    res = res + 1;
    return res;
}

int third() {
    int res;
    res = second();
    res = res + 2;
    return res;
}

int fourth() {
    int res;
    res = third();
    res = res + 3;
    return res;
}

int fifth() {
    int res;
    res = fourth();
    res = res + 4;
    return res;
}

int sixth() {
    int res;
    res = fifth();
    res = res + 5;
    return res;
}

int main() {
    int res = 0;
    int buf;
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
