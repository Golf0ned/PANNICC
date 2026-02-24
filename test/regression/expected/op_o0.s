    .text
    .globl  add
    .type   add, @function
add:
        subq    $28, %rsp
.Ladd_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Ladd_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Ladd_end:
    .globl  sub
    .type   sub, @function
sub:
        subq    $28, %rsp
.Lsub_entry:
        movl    $10, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lsub_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lsub_end:
    .globl  mul
    .type   mul, @function
mul:
        subq    $28, %rsp
.Lmul_entry:
        movl    $3, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lmul_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lmul_end:
    .globl  div
    .type   div, @function
div:
        subq    $28, %rsp
.Ldiv_entry:
        movl    $24, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $6, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %r11d
        movl    %r10d, %eax
        cltd
        idivl   %r11d
        movl    %eax, %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Ldiv_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Ldiv_end:
    .globl  lshift
    .type   lshift, @function
lshift:
        subq    $28, %rsp
.Llshift_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %r11d
        movl    %r10d, %ecx
        shll    %r11d, %ecx
        movl    %ecx, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Llshift_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Llshift_end:
    .globl  rshift
    .type   rshift, @function
rshift:
        subq    $28, %rsp
.Lrshift_entry:
        movl    $16, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %r11d
        movl    %r10d, %ecx
        sarl    %r11d, %ecx
        movl    %ecx, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lrshift_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lrshift_end:
    .globl  bit_and
    .type   bit_and, @function
bit_and:
        subq    $28, %rsp
.Lbit_and_entry:
        movl    $31, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %edi
        movl    %r10d, %r11d
        andl    %edi, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lbit_and_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lbit_and_end:
    .globl  bit_or
    .type   bit_or, @function
bit_or:
        subq    $28, %rsp
.Lbit_or_entry:
        movl    $12, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %edi
        movl    %r10d, %r11d
        orl     %edi, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lbit_or_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lbit_or_end:
    .globl  bit_xor
    .type   bit_xor, @function
bit_xor:
        subq    $28, %rsp
.Lbit_xor_entry:
        movl    $15, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %edi
        movl    %r10d, %r11d
        xorl    %edi, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lbit_xor_15:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lbit_xor_end:
    .globl  unary_plus
    .type   unary_plus, @function
unary_plus:
        subq    $16, %rsp
.Lunary_plus_entry:
        movl    $42, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lunary_plus_8:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lunary_plus_end:
    .globl  unary_minus
    .type   unary_minus, @function
unary_minus:
        subq    $16, %rsp
.Lunary_minus_entry:
        movl    $42, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r11d
        movl    $0, %r10d
        subl    %r11d, %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lunary_minus_9:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lunary_minus_end:
    .globl  unary_bitwise_not
    .type   unary_bitwise_not, @function
unary_bitwise_not:
        subq    $16, %rsp
.Lunary_bitwise_not_entry:
        movl    $42, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, %r11d
        xorl    $18446744073709551615, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lunary_bitwise_not_9:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lunary_bitwise_not_end:
    .globl  add_equals
    .type   add_equals, @function
add_equals:
        subq    $20, %rsp
.Ladd_equals_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, 8(%rsp)
        movl    (%rsp), %edi
        movl    8(%rsp), %r10d
        leal    (%edi,%r10d), %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Ladd_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Ladd_equals_end:
    .globl  sub_equals
    .type   sub_equals, @function
sub_equals:
        subq    $20, %rsp
.Lsub_equals_entry:
        movl    $10, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lsub_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lsub_equals_end:
    .globl  mul_equals
    .type   mul_equals, @function
mul_equals:
        subq    $20, %rsp
.Lmul_equals_entry:
        movl    $3, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lmul_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lmul_equals_end:
    .globl  div_equals
    .type   div_equals, @function
div_equals:
        subq    $20, %rsp
.Ldiv_equals_entry:
        movl    $24, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $6, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %r11d
        movl    %r10d, %eax
        cltd
        idivl   %r11d
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Ldiv_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Ldiv_equals_end:
    .globl  lshift_equals
    .type   lshift_equals, @function
lshift_equals:
        subq    $20, %rsp
.Llshift_equals_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %r11d
        movl    %r10d, %ecx
        shll    %r11d, %ecx
        movl    %ecx, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Llshift_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Llshift_equals_end:
    .globl  rshift_equals
    .type   rshift_equals, @function
rshift_equals:
        subq    $20, %rsp
.Lrshift_equals_entry:
        movl    $16, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %r11d
        movl    %r10d, %ecx
        sarl    %r11d, %ecx
        movl    %ecx, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lrshift_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lrshift_equals_end:
    .globl  bit_and_equals
    .type   bit_and_equals, @function
bit_and_equals:
        subq    $20, %rsp
.Lbit_and_equals_entry:
        movl    $31, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %edi
        movl    %r10d, %r11d
        andl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lbit_and_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lbit_and_equals_end:
    .globl  bit_or_equals
    .type   bit_or_equals, @function
bit_or_equals:
        subq    $20, %rsp
.Lbit_or_equals_entry:
        movl    $12, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %edi
        movl    %r10d, %r11d
        orl     %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lbit_or_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lbit_or_equals_end:
    .globl  bit_xor_equals
    .type   bit_xor_equals, @function
bit_xor_equals:
        subq    $20, %rsp
.Lbit_xor_equals_entry:
        movl    $15, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %edi
        movl    %r10d, %r11d
        xorl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lbit_xor_equals_11:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lbit_xor_equals_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"