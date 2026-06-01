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
    .globl  simplify
    .type   simplify, @function
simplify:
        movl    %edi, %edi
.Lsimplify_entry:
        movl    $0, %r10d
        imull   %edi, %r10d
        movl    %edi, %r10d
        imull   $0, %r10d
        movl    %edi, %r11d
        addl    $0, %r11d
        movl    $0, %r10d
        addl    %r11d, %r10d
        movl    %r10d, %r11d
        subl    $0, %r11d
        movl    %r11d, %r10d
        movl    %r10d, %r11d
        movl    %r11d, %eax
        cltd
        idivl   $1
        movl    %eax, %r10d
        movl    $18446744073709551615, %r11d
        andl    %r10d, %r11d
        movl    %r11d, %r10d
        andl    $18446744073709551615, %r10d
        movl    $0, %r11d
        orl     %r10d, %r11d
        movl    %r11d, %r10d
        orl     $0, %r10d
        movl    %r10d, %ecx
        shll    $0, %ecx
        movl    %ecx, %ecx
        sarl    $0, %ecx
.Lsimplify_15:
        movl    %ecx, %eax
        ret
.Lsimplify_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"