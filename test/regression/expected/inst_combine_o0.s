    .text
    .globl  constant_fold
    .type   constant_fold, @function
constant_fold:
        subq    $44, %rsp
.Lconstant_fold_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $13, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %edi
        movl    4(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    16(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    4(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lconstant_fold_31:
        movl    (%rsp), %r10d
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
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lconstant_fold_overflow_6:
        movl    (%rsp), %r10d
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
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lconstant_fold_underflow_6:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lconstant_fold_underflow_end:
    .globl  simplify
    .type   simplify, @function
simplify:
        subq    $72, %rsp
        movl    %edi, %r10d
.Lsimplify_entry:
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r11d
        movl    $0, %r10d
        imull   %r11d, %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        imull   $0, %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $0, %r10d
        movl    %r10d, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r11d
        movl    $0, %r10d
        addl    %r11d, %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %r11d
        subl    $0, %r11d
        movl    %r11d, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %r11d
        movl    %r11d, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %r11d
        movl    %r11d, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        cltd
        idivl   $1
        movl    %eax, %r10d
        movl    %r10d, 44(%rsp)
        movl    44(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r11d
        movl    $18446744073709551615, %r10d
        andl    %r11d, %r10d
        movl    %r10d, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %r11d
        andl    $18446744073709551615, %r11d
        movl    %r11d, 52(%rsp)
        movl    52(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r11d
        movl    $0, %r10d
        orl     %r11d, %r10d
        movl    %r10d, 56(%rsp)
        movl    56(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %r11d
        orl     $0, %r11d
        movl    %r11d, 60(%rsp)
        movl    60(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %ecx
        shll    $0, %ecx
        movl    %ecx, 64(%rsp)
        movl    64(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, %ecx
        sarl    $0, %ecx
        movl    %ecx, 68(%rsp)
        movl    68(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lsimplify_62:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $72, %rsp
        ret
.Lsimplify_end:
    .globl  copy_propagation
    .type   copy_propagation, @function
copy_propagation:
        subq    $36, %rsp
        movl    %edi, %r10d
.Lcopy_propagation_entry:
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r11d
        leal    (,%r11d,4), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lcopy_propagation_19
.Lcopy_propagation_14:
.Lcopy_propagation_15:
        movl    12(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lcopy_propagation_20:
        movl    4(%rsp), %r10d
        movl    12(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lcopy_propagation_26:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lcopy_propagation_19:
        addq    $36, %rsp
        jmp     .Lcopy_propagation_20
.Lcopy_propagation_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"