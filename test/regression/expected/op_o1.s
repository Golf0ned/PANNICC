add:
.add_entry:
        movl    $3, %eax
        ret
sub:
.sub_entry:
        movl    $9, %eax
        ret
mul:
.mul_entry:
        movl    $12, %eax
        ret
div:
.div_entry:
        movl    $4, %eax
        ret
lshift:
.lshift_entry:
        movl    $16, %eax
        ret
rshift:
.rshift_entry:
        movl    $2, %eax
        ret
bit_and:
.bit_and_entry:
        movl    $4, %eax
        ret
bit_or:
.bit_or_entry:
        movl    $15, %eax
        ret
bit_xor:
.bit_xor_entry:
        movl    $11, %eax
        ret
unary_plus:
.unary_plus_entry:
        movl    $42, %eax
        ret
unary_minus:
.unary_minus_entry:
        movl    $18446744073709551574, %eax
        ret
unary_bitwise_not:
.unary_bitwise_not_entry:
        movl    $18446744073709551573, %eax
        ret
add_equals:
.add_equals_entry:
        movl    $3, %eax
        ret
sub_equals:
.sub_equals_entry:
        movl    $8, %eax
        ret
mul_equals:
.mul_equals_entry:
        movl    $12, %eax
        ret
div_equals:
.div_equals_entry:
        movl    $4, %eax
        ret
lshift_equals:
.lshift_equals_entry:
        movl    $16, %eax
        ret
rshift_equals:
.rshift_equals_entry:
        movl    $2, %eax
        ret
bit_and_equals:
.bit_and_equals_entry:
        movl    $4, %eax
        ret
bit_or_equals:
.bit_or_equals_entry:
        movl    $15, %eax
        ret
bit_xor_equals:
.bit_xor_equals_entry:
        movl    $11, %eax
        ret