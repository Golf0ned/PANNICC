int fibonacci(int n) {
    if (n) {
        if (n - 1)
            return fibonacci(n - 1) + fibonacci(n - 2);
        return 1;
    }
    return 0;
}

int main() {
    return fibonacci(42);
}
