int fib_rec(int n) {
    if (n) {
        if (n - 1) {
            return fib_rec(n - 1) + fib_rec(n - 2);
        }
        return 1;
    }
    return 0;
}

int fib_it(int n) {
    int first = 0;
    int second = 1;
    int temp;

    while (n) {
        temp = first + second;
        first = second;
        second = temp;
        n -= 1;
    }

    return first;
}

int main() {
    return fib_rec(10) + fib_it(1000);
}
