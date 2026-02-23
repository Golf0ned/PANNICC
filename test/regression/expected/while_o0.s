while_basic:
        subq    $28, %rsp
.Lwhile_basic_entry:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lwhile_basic_8:
        movl    $1, $8(%rsp)
        movl    $8(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .Lwhile_basic_17
.Lwhile_basic_10:
.Lwhile_basic_11:
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $2, $16(%rsp)
        movl    $12(%rsp), %r10d
        movl    $16(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, (%rsp)
        jmp     .Lwhile_basic_8
.Lwhile_basic_17:
.Lwhile_basic_18:
        movl    (%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.Lwhile_basic_21:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
while_complex:
        subq    $44, %rsp
.Lwhile_complex_entry:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $100, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
.Lwhile_complex_13:
        movl    $8(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .Lwhile_complex_28
.Lwhile_complex_16:
.Lwhile_complex_17:
        movl    (%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $5, $24(%rsp)
        movl    $20(%rsp), %r10d
        movl    $24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $1, $32(%rsp)
        movl    $28(%rsp), %r10d
        movl    $32(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $28(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        jmp     .Lwhile_complex_13
.Lwhile_complex_28:
.Lwhile_complex_29:
        movl    (%rsp), %r10d
        movl    %r10d, $36(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $40(%rsp)
.Lwhile_complex_32:
        movl    $40(%rsp), %r10d
        movl    %r10d, %eax
        addq    $44, %rsp
        ret
while_empty:
        subq    $12, %rsp
.Lwhile_empty_entry:
.Lwhile_empty_3:
        movl    $0, (%rsp)
        movl    (%rsp), %r10d
        cmpl    %r10d, $0
        jne     .Lwhile_empty_7
.Lwhile_empty_5:
.Lwhile_empty_6:
        jmp     .Lwhile_empty_3
.Lwhile_empty_7:
.Lwhile_empty_8:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $8(%rsp)
.Lwhile_empty_10:
        movl    $8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
while_if:
        subq    $28, %rsp
.Lwhile_if_entry:
        movl    $200, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lwhile_if_8:
        movl    $1, $8(%rsp)
        movl    $8(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .Lwhile_if_20
.Lwhile_if_10:
.Lwhile_if_11:
        movl    (%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .Lwhile_if_16
.Lwhile_if_14:
.Lwhile_if_15:
.Lwhile_if_19:
        jmp     .Lwhile_if_8
.Lwhile_if_20:
.Lwhile_if_21:
        movl    $0, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.Lwhile_if_23:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lwhile_if_16:
.Lwhile_if_17:
        movl    $500, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        addq    $28, %rsp
        jmp     .Lwhile_if_23