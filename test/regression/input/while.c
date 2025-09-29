int64_t while_basic() {
    int64_t val = 0;
    while (1) {
        val = val + 2;
    }
    return val;
}

int64_t while_complex() {
    int64_t res = 0;
    int64_t cond = 100;
    while (cond) {
        res = res + 5;
        cond = cond - 1;
    }
    return res;
}

int64_t while_empty() {
    while (0) {
    }
    return 0;
}

int64_t while_if() {
    int64_t counter = 200;
    while (1) {
        if (counter) {
        } else {
            return 500;
        }
    }
    return 0;
}
