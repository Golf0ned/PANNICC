    .text
    .globl  add
    .type   add, @function
add:
        subq    $16, %rsp
.Ladd_entry:
        movl    $1, 4(%rsp)
        movl    $2, 8(%rsp)
        movl    4(%rsp), %edi
        movl    8(%rsp), %r10d
        leal    (%edi,%r10d), %r11d
        movl    %r11d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Ladd_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Ladd_end:
    .globl  sub
    .type   sub, @function
sub:
        subq    $16, %rsp
.Lsub_entry:
        movl    $10, 4(%rsp)
        movl    $1, 8(%rsp)
        movl    4(%rsp), %edi
        movl    8(%rsp), %r11d
        movl    %edi, %r10d
        subl    %r11d, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lsub_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lsub_end:
    .globl  mul
    .type   mul, @function
mul:
        subq    $16, %rsp
.Lmul_entry:
        movl    $3, 4(%rsp)
        movl    $4, 8(%rsp)
        movl    4(%rsp), %edi
        movl    8(%rsp), %r11d
        movl    %edi, %r10d
        imull   %r11d, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lmul_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lmul_end:
    .globl  div
    .type   div, @function
div:
        subq    $16, %rsp
.Ldiv_entry:
        movl    $24, 4(%rsp)
        movl    $6, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
        movl    %r11d, %eax
        cltd
        idivl   %r10d
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Ldiv_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Ldiv_end:
    .globl  lshift
    .type   lshift, @function
lshift:
        subq    $16, %rsp
.Llshift_entry:
        movl    $1, 4(%rsp)
        movl    $4, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
        movl    %r11d, %ecx
        shll    %r10d, %ecx
        movl    %ecx, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Llshift_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Llshift_end:
    .globl  rshift
    .type   rshift, @function
rshift:
        subq    $16, %rsp
.Lrshift_entry:
        movl    $16, 4(%rsp)
        movl    $3, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
        movl    %r11d, %ecx
        sarl    %r10d, %ecx
        movl    %ecx, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lrshift_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lrshift_end:
    .globl  bit_and
    .type   bit_and, @function
bit_and:
        subq    $16, %rsp
.Lbit_and_entry:
        movl    $31, 4(%rsp)
        movl    $4, 8(%rsp)
        movl    4(%rsp), %edi
        movl    8(%rsp), %r11d
        movl    %edi, %r10d
        andl    %r11d, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lbit_and_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lbit_and_end:
    .globl  bit_or
    .type   bit_or, @function
bit_or:
        subq    $16, %rsp
.Lbit_or_entry:
        movl    $12, 4(%rsp)
        movl    $3, 8(%rsp)
        movl    4(%rsp), %edi
        movl    8(%rsp), %r11d
        movl    %edi, %r10d
        orl     %r11d, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lbit_or_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lbit_or_end:
    .globl  bit_xor
    .type   bit_xor, @function
bit_xor:
        subq    $16, %rsp
.Lbit_xor_entry:
        movl    $15, 4(%rsp)
        movl    $4, 8(%rsp)
        movl    4(%rsp), %edi
        movl    8(%rsp), %r11d
        movl    %edi, %r10d
        xorl    %r11d, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lbit_xor_8:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lbit_xor_end:
    .globl  unary_plus
    .type   unary_plus, @function
unary_plus:
        subq    $12, %rsp
.Lunary_plus_entry:
        movl    $42, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lunary_plus_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lunary_plus_end:
    .globl  unary_minus
    .type   unary_minus, @function
unary_minus:
        subq    $12, %rsp
.Lunary_minus_entry:
        movl    $42, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    $0, %r11d
        subl    %r10d, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lunary_minus_6:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lunary_minus_end:
    .globl  unary_bitwise_not
    .type   unary_bitwise_not, @function
unary_bitwise_not:
        subq    $12, %rsp
.Lunary_bitwise_not_entry:
        movl    $42, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        xorl    $18446744073709551615, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lunary_bitwise_not_6:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lunary_bitwise_not_end:
    .globl  add_equals
    .type   add_equals, @function
add_equals:
        subq    $8, %rsp
.Ladd_equals_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $2, %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Ladd_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Ladd_equals_end:
    .globl  sub_equals
    .type   sub_equals, @function
sub_equals:
        subq    $8, %rsp
.Lsub_equals_entry:
        movl    $10, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        subl    $2, %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lsub_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lsub_equals_end:
    .globl  mul_equals
    .type   mul_equals, @function
mul_equals:
        subq    $8, %rsp
.Lmul_equals_entry:
        movl    $3, 4(%rsp)
        movl    4(%rsp), %r11d
        leal    (,%r11d,4), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lmul_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lmul_equals_end:
    .globl  div_equals
    .type   div_equals, @function
div_equals:
        subq    $8, %rsp
.Ldiv_equals_entry:
        movl    $24, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        cltd
        idivl   $6
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Ldiv_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Ldiv_equals_end:
    .globl  lshift_equals
    .type   lshift_equals, @function
lshift_equals:
        subq    $8, %rsp
.Llshift_equals_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %ecx
        shll    $4, %ecx
        movl    %ecx, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Llshift_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Llshift_equals_end:
    .globl  rshift_equals
    .type   rshift_equals, @function
rshift_equals:
        subq    $8, %rsp
.Lrshift_equals_entry:
        movl    $16, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %ecx
        sarl    $3, %ecx
        movl    %ecx, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lrshift_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lrshift_equals_end:
    .globl  bit_and_equals
    .type   bit_and_equals, @function
bit_and_equals:
        subq    $8, %rsp
.Lbit_and_equals_entry:
        movl    $31, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        andl    $4, %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lbit_and_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lbit_and_equals_end:
    .globl  bit_or_equals
    .type   bit_or_equals, @function
bit_or_equals:
        subq    $8, %rsp
.Lbit_or_equals_entry:
        movl    $12, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        orl     $3, %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lbit_or_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lbit_or_equals_end:
    .globl  bit_xor_equals
    .type   bit_xor_equals, @function
bit_xor_equals:
        subq    $8, %rsp
.Lbit_xor_equals_entry:
        movl    $15, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        xorl    $4, %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lbit_xor_equals_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lbit_xor_equals_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"