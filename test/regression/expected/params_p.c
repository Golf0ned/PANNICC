int fun1(int a) { return a * a; }

int fun2(int a, int b) { return fun1(a) + 2 * a * b + fun1(b); }

int fun3(int a, int b, int c, int x) { return a * fun1(x) + b * x + c; }

int call1() { return fun1(5); }

int call2(int foo, int bar) { return fun2(foo, bar); }

int call3(int foo, int bar) { return fun3(foo, bar, 1, 0); }

int call4() { return fun1(10) - 100; }

int super_fun(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

int call_super_fun(int seventh, int eighth) {
    return super_fun(1, 2, 3, 4, 5, 6, seventh, eighth);
}

int caller_saved(int first, int second) {
    int p1 = first + 1;
    int p2 = first + 2;
    int p3 = first + 3;
    int p4 = first + 4;
    int p5 = first + 5;
    int p6 = first + 6;
    int p7 = first + 7;
    int p8 = first + 8;
    int p9 = first + 9;
    int p10 = first + 10;
    int p11 = first + 11;

    int superfun = super_fun(p1, p2, p3, p4, p5, p6, p7, p8);

    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + superfun +
           second;
}
