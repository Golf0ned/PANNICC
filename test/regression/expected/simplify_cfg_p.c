int simplify_cfg_delete_orphaned() {
    if (1)
        return 2;
    else
        return 3;
}

int simplify_cfg_merge_straight_line() {
    int val = 0;
    if (1) {
        if (2) {
            return 3;
        }
        val = val + 4;
    } else {
        return 5;
    }
    return val;
}

int simplify_cfg_delete_empty() {
    if (1) {
        if (2) {
            
        } else {

        }
    } else {
        if (3) {

        } else {

        }
    }
    return 4;
}

int simplify_cfg_empty_loop() {
    while(0) {}
    return 0;
}
