    .text
    .globl  constant_fold
    .type   constant_fold, @function
constant_fold:
.Lconstant_fold_entry:
.Lconstant_fold_0:
        movl    $55, %eax
        ret
.Lconstant_fold_end:
    .globl  constant_fold_overflow
    .type   constant_fold_overflow, @function
constant_fold_overflow:
.Lconstant_fold_overflow_entry:
.Lconstant_fold_overflow_0:
        movl    $18446744071562067968, %eax
        ret
.Lconstant_fold_overflow_end:
    .globl  constant_fold_underflow
    .type   constant_fold_underflow, @function
constant_fold_underflow:
.Lconstant_fold_underflow_entry:
.Lconstant_fold_underflow_0:
        movl    $2147483647, %eax
        ret
.Lconstant_fold_underflow_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"