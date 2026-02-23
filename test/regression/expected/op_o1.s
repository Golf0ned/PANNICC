add:
.Ladd_entry:
        movl    $3, %eax
        ret
sub:
.Lsub_entry:
        movl    $9, %eax
        ret
mul:
.Lmul_entry:
        movl    $12, %eax
        ret
div:
.Ldiv_entry:
        movl    $4, %eax
        ret
lshift:
.Llshift_entry:
        movl    $16, %eax
        ret
rshift:
.Lrshift_entry:
        movl    $2, %eax
        ret
bit_and:
.Lbit_and_entry:
        movl    $4, %eax
        ret
bit_or:
.Lbit_or_entry:
        movl    $15, %eax
        ret
bit_xor:
.Lbit_xor_entry:
        movl    $11, %eax
        ret
unary_plus:
.Lunary_plus_entry:
        movl    $42, %eax
        ret
unary_minus:
.Lunary_minus_entry:
        movl    $18446744073709551574, %eax
        ret
unary_bitwise_not:
.Lunary_bitwise_not_entry:
        movl    $18446744073709551573, %eax
        ret
add_equals:
.Ladd_equals_entry:
        movl    $3, %eax
        ret
sub_equals:
.Lsub_equals_entry:
        movl    $8, %eax
        ret
mul_equals:
.Lmul_equals_entry:
        movl    $12, %eax
        ret
div_equals:
.Ldiv_equals_entry:
        movl    $4, %eax
        ret
lshift_equals:
.Llshift_equals_entry:
        movl    $16, %eax
        ret
rshift_equals:
.Lrshift_equals_entry:
        movl    $2, %eax
        ret
bit_and_equals:
.Lbit_and_equals_entry:
        movl    $4, %eax
        ret
bit_or_equals:
.Lbit_or_equals_entry:
        movl    $15, %eax
        ret
bit_xor_equals:
.Lbit_xor_equals_entry:
        movl    $11, %eax
        ret