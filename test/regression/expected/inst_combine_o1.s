    .text
    .globl  constant_fold
    .type   constant_fold, @function
constant_fold:
.Lconstant_fold_entry:
        movl    $55, %eax
        ret
.Lconstant_fold_end:
    .globl  constant_fold_overflow
    .type   constant_fold_overflow, @function
constant_fold_overflow:
.Lconstant_fold_overflow_entry:
        movl    $18446744071562067968, %eax
        ret
.Lconstant_fold_overflow_end:
    .globl  constant_fold_underflow
    .type   constant_fold_underflow, @function
constant_fold_underflow:
.Lconstant_fold_underflow_entry:
        movl    $2147483647, %eax
        ret
.Lconstant_fold_underflow_end:
    .globl  simplify
    .type   simplify, @function
simplify:
        movl    %edi, %r10d
.Lsimplify_entry:
        movl    %r10d, %eax
        ret
.Lsimplify_end:
    .globl  copy_propagation
    .type   copy_propagation, @function
copy_propagation:
        movl    %edi, %r11d
.Lcopy_propagation_entry:
        leal    (,%r11d,4), %r10d
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lcopy_propagation_5
.Lcopy_propagation_2:
.Lcopy_propagation_3:
        movl    %r11d, %r10d
        addl    $2, %r10d
.Lcopy_propagation_6:
        leal    (%r11d,%r11d), %r10d
        movl    %r10d, %eax
        ret
.Lcopy_propagation_5:
        jmp     .Lcopy_propagation_6
.Lcopy_propagation_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"