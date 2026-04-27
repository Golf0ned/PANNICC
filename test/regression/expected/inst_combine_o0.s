    .text
    .globl  constant_fold
    .type   constant_fold, @function
constant_fold:
        subq    $44, %rsp
.Lconstant_fold_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r11d
        movl    %r11d, %r10d
        addl    $13, %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    (%rsp), %edi
        movl    (%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r10d
        movl    12(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    (%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 40(%rsp)
.Lconstant_fold_31:
        movl    40(%rsp), %r10d
        movl    %r10d, %eax
        addq    $44, %rsp
        ret
.Lconstant_fold_end:
    .globl  constant_fold_overflow
    .type   constant_fold_overflow, @function
constant_fold_overflow:
        subq    $12, %rsp
.Lconstant_fold_overflow_entry:
        movl    $2147483647, %r10d
        addl    $1, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lconstant_fold_overflow_6:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lconstant_fold_overflow_end:
    .globl  constant_fold_underflow
    .type   constant_fold_underflow, @function
constant_fold_underflow:
        subq    $12, %rsp
.Lconstant_fold_underflow_entry:
        movl    $18446744071562067968, %r10d
        subl    $1, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lconstant_fold_underflow_6:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lconstant_fold_underflow_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"