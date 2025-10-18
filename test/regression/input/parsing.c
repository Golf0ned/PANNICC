// default return
int main() {}

int compound() {
    int i = 9 * 123 + 3 & 123 - 122;
    int j = 1 + 2 * 3 - 4;
    return i + 2 + 3 + 4 + 5 + 6 + j;
}

int parens() {
    return 4 * (3 - (2 - 1));
}
