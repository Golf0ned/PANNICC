int first() { return 1; }
int second() { return first() + 1; }
int third() { return second() + 2; }
int fourth() { return third() + 3; }
int fifth() { return fourth() + 4; }
int sixth() { return fifth() + 5; }

int main() {
    sixth();

    int res = 0;
    res = res + first();
    res = res + second();
    res = res + third();
    res = res + fourth();
    res = res + fifth();
    res = res + sixth();
    res = res + first() + second() + third() + fourth() + fifth() + sixth();
    return res;
}
