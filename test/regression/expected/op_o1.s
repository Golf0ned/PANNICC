    .text
    .globl  add
    .type   add, @function
add:
.Ladd_entry:
        movl    $3, %eax
        ret
.Ladd_end:
    .globl  sub
    .type   sub, @function
sub:
.Lsub_entry:
        movl    $9, %eax
        ret
.Lsub_end:
    .globl  mul
    .type   mul, @function
mul:
.Lmul_entry:
        movl    $12, %eax
        ret
.Lmul_end:
    .globl  div
    .type   div, @function
div:
.Ldiv_entry:
        movl    $4, %eax
        ret
.Ldiv_end:
    .globl  lshift
    .type   lshift, @function
lshift:
.Llshift_entry:
        movl    $16, %eax
        ret
.Llshift_end:
    .globl  rshift
    .type   rshift, @function
rshift:
.Lrshift_entry:
        movl    $2, %eax
        ret
.Lrshift_end:
    .globl  bit_and
    .type   bit_and, @function
bit_and:
.Lbit_and_entry:
        movl    $4, %eax
        ret
.Lbit_and_end:
    .globl  bit_or
    .type   bit_or, @function
bit_or:
.Lbit_or_entry:
        movl    $15, %eax
        ret
.Lbit_or_end:
    .globl  bit_xor
    .type   bit_xor, @function
bit_xor:
.Lbit_xor_entry:
        movl    $11, %eax
        ret
.Lbit_xor_end:
    .globl  unary_plus
    .type   unary_plus, @function
unary_plus:
.Lunary_plus_entry:
        movl    $42, %eax
        ret
.Lunary_plus_end:
    .globl  unary_minus
    .type   unary_minus, @function
unary_minus:
.Lunary_minus_entry:
        movl    $18446744073709551574, %eax
        ret
.Lunary_minus_end:
    .globl  unary_bitwise_not
    .type   unary_bitwise_not, @function
unary_bitwise_not:
.Lunary_bitwise_not_entry:
        movl    $18446744073709551573, %eax
        ret
.Lunary_bitwise_not_end:
    .globl  add_equals
    .type   add_equals, @function
add_equals:
.Ladd_equals_entry:
        movl    $3, %eax
        ret
.Ladd_equals_end:
    .globl  sub_equals
    .type   sub_equals, @function
sub_equals:
.Lsub_equals_entry:
        movl    $8, %eax
        ret
.Lsub_equals_end:
    .globl  mul_equals
    .type   mul_equals, @function
mul_equals:
.Lmul_equals_entry:
        movl    $12, %eax
        ret
.Lmul_equals_end:
    .globl  div_equals
    .type   div_equals, @function
div_equals:
.Ldiv_equals_entry:
        movl    $4, %eax
        ret
.Ldiv_equals_end:
    .globl  lshift_equals
    .type   lshift_equals, @function
lshift_equals:
.Llshift_equals_entry:
        movl    $16, %eax
        ret
.Llshift_equals_end:
    .globl  rshift_equals
    .type   rshift_equals, @function
rshift_equals:
.Lrshift_equals_entry:
        movl    $2, %eax
        ret
.Lrshift_equals_end:
    .globl  bit_and_equals
    .type   bit_and_equals, @function
bit_and_equals:
.Lbit_and_equals_entry:
        movl    $4, %eax
        ret
.Lbit_and_equals_end:
    .globl  bit_or_equals
    .type   bit_or_equals, @function
bit_or_equals:
.Lbit_or_equals_entry:
        movl    $15, %eax
        ret
.Lbit_or_equals_end:
    .globl  bit_xor_equals
    .type   bit_xor_equals, @function
bit_xor_equals:
.Lbit_xor_equals_entry:
        movl    $11, %eax
        ret
.Lbit_xor_equals_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"