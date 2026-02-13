constant_fold:
.constant_fold_entry:
        movl    $55, %eax
        ret
constant_fold_overflow:
.constant_fold_overflow_entry:
        movl    $18446744071562067968, %eax
        ret
constant_fold_underflow:
.constant_fold_underflow_entry:
        movl    $2147483647, %eax
        ret