add:
        subq    $32, %rsp
.add_entry:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.add_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
sub:
        subq    $32, %rsp
.sub_entry:
        movl    $10, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.sub_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
mul:
        subq    $32, %rsp
.mul_entry:
        movl    $3, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.mul_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
div:
        subq    $32, %rsp
.div_entry:
        movl    $24, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $6, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %r11d
        movl    %r10d, %eax
        cltd
        idivl   %r11d
        movl    %eax, %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.div_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
lshift:
        subq    $32, %rsp
.lshift_entry:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %r11d
        movl    %r10d, %ecx
        shll    %r11d, %ecx
        movl    %ecx, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.lshift_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
rshift:
        subq    $32, %rsp
.rshift_entry:
        movl    $16, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %r11d
        movl    %r10d, %ecx
        sarl    %r11d, %ecx
        movl    %ecx, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.rshift_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
bit_and:
        subq    $32, %rsp
.bit_and_entry:
        movl    $31, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %edi
        movl    %r10d, %r11d
        andl    %edi, %r11d
        movl    %r11d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.bit_and_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
bit_or:
        subq    $32, %rsp
.bit_or_entry:
        movl    $12, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %edi
        movl    %r10d, %r11d
        orl     %edi, %r11d
        movl    %r11d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.bit_or_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
bit_xor:
        subq    $32, %rsp
.bit_xor_entry:
        movl    $15, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %edi
        movl    %r10d, %r11d
        xorl    %edi, %r11d
        movl    %r11d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.bit_xor_15:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
unary_plus:
        subq    $16, %rsp
.unary_plus_entry:
        movl    $42, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $12(%rsp)
.unary_plus_8:
        movl    $12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
unary_minus:
        subq    $16, %rsp
.unary_minus_entry:
        movl    $42, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $8(%rsp), %r11d
        movl    $0, %r10d
        subl    %r11d, %r10d
        movl    %r10d, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $12(%rsp)
.unary_minus_9:
        movl    $12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
unary_bitwise_not:
        subq    $16, %rsp
.unary_bitwise_not_entry:
        movl    $42, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, %r11d
        xorl    $18446744073709551615, %r11d
        movl    %r11d, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $12(%rsp)
.unary_bitwise_not_9:
        movl    $12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
add_equals:
        subq    $32, %rsp
.add_equals_entry:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, $8(%rsp)
        movl    (%rsp), %edi
        movl    $8(%rsp), %r10d
        leal    (%edi,%r10d), %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.add_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
sub_equals:
        subq    $32, %rsp
.sub_equals_entry:
        movl    $10, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.sub_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
mul_equals:
        subq    $32, %rsp
.mul_equals_entry:
        movl    $3, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.mul_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
div_equals:
        subq    $32, %rsp
.div_equals_entry:
        movl    $24, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $6, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %r11d
        movl    %r10d, %eax
        cltd
        idivl   %r11d
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.div_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
lshift_equals:
        subq    $32, %rsp
.lshift_equals_entry:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %r11d
        movl    %r10d, %ecx
        shll    %r11d, %ecx
        movl    %ecx, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.lshift_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
rshift_equals:
        subq    $32, %rsp
.rshift_equals_entry:
        movl    $16, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %r11d
        movl    %r10d, %ecx
        sarl    %r11d, %ecx
        movl    %ecx, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.rshift_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
bit_and_equals:
        subq    $32, %rsp
.bit_and_equals_entry:
        movl    $31, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        andl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.bit_and_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
bit_or_equals:
        subq    $32, %rsp
.bit_or_equals_entry:
        movl    $12, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        orl     %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.bit_or_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
bit_xor_equals:
        subq    $32, %rsp
.bit_xor_equals_entry:
        movl    $15, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $4, $8(%rsp)
        movl    (%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        xorl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.bit_xor_equals_11:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret