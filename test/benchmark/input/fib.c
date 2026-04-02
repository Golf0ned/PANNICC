int fibonacci(int n) {
    if (n) {
        if (n - 1)
            return fibonacci(n - 1) + fibonacci(n - 2);
        return 1;
    }
    return 0;
}

int main() {
    int expected = 267914296;
    int actual = fibonacci(42);

    if (expected - actual)
        return 1;
    return 0;
}
