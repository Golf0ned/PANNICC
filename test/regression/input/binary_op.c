int add() {
    int a = 1;
    int b = 2;
    return a + b;
}

int sub() {
    int a = 10;
    int b = 1;
    return a - b;
}

int mul() {
    int a = 3;
    int b = 4;
    return a * b;
}

int div() {
    int a = 24;
    int b = 6;
    return a / b;
}

int bit_and() {
    int a = 31;
    int b = 4;
    return a & b;
}

int bit_or() {
    int a = 12;
    int b = 3;
    return a | b;
}

int bit_xor() {
    int a = 15;
    int b = 4;
    return a ^ b;
}

int unary_plus() {
    int a = 42;
    return +a;
}

int unary_minus() {
    int a = 42;
    return -a;
}

int unary_bitwise_not() {
    int a = 42;
    return ~a;
}
