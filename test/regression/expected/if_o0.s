    .text
    .globl  if_basic
    .type   if_basic, @function
if_basic:
        subq    $28, %rsp
.Lif_basic_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_basic_17
.Lif_basic_10:
.Lif_basic_11:
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    $100, 16(%rsp)
        movl    12(%rsp), %r10d
        movl    16(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lif_basic_18:
        movl    (%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lif_basic_21:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lif_basic_17:
        addq    $28, %rsp
        jmp     .Lif_basic_18
.Lif_basic_end:
    .globl  if_basic_else
    .type   if_basic_else, @function
if_basic_else:
        subq    $36, %rsp
.Lif_basic_else_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_basic_else_19
.Lif_basic_else_12:
.Lif_basic_else_13:
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    $3, 16(%rsp)
        movl    12(%rsp), %r11d
        movl    16(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lif_basic_else_26:
        movl    (%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 32(%rsp)
.Lif_basic_else_29:
        movl    32(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lif_basic_else_19:
.Lif_basic_else_20:
        movl    (%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    $100, 24(%rsp)
        movl    20(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, (%rsp)
        addq    $36, %rsp
        jmp     .Lif_basic_else_26
.Lif_basic_else_end:
    .globl  if_no_braces
    .type   if_no_braces, @function
if_no_braces:
        subq    $24, %rsp
.Lif_no_braces_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_no_braces_13
.Lif_no_braces_9:
.Lif_no_braces_10:
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 20(%rsp)
.Lif_no_braces_18:
        movl    20(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lif_no_braces_13:
.Lif_no_braces_14:
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        addq    $24, %rsp
        jmp     .Lif_no_braces_18
.Lif_no_braces_end:
    .globl  if_chain
    .type   if_chain, @function
if_chain:
        subq    $36, %rsp
.Lif_chain_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_chain_22
.Lif_chain_13:
.Lif_chain_14:
        movl    8(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_chain_20
.Lif_chain_17:
.Lif_chain_18:
        movl    $0, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 32(%rsp)
.Lif_chain_25:
        movl    32(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lif_chain_22:
.Lif_chain_23:
        movl    $1, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        jmp     .Lif_chain_25
.Lif_chain_20:
.Lif_chain_21:
        addq    $36, %rsp
        jmp     .Lif_chain_23
.Lif_chain_end:
    .globl  if_chain_many
    .type   if_chain_many, @function
if_chain_many:
        subq    $52, %rsp
.Lif_chain_many_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    $0, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_chain_many_35
.Lif_chain_many_18:
.Lif_chain_many_19:
        movl    8(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    28(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_chain_many_33
.Lif_chain_many_22:
.Lif_chain_many_23:
        movl    16(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_chain_many_29
.Lif_chain_many_26:
.Lif_chain_many_27:
        movl    $0, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 48(%rsp)
.Lif_chain_many_38:
        movl    48(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lif_chain_many_35:
.Lif_chain_many_36:
        movl    $1, 44(%rsp)
        movl    44(%rsp), %r10d
        movl    %r10d, 48(%rsp)
        jmp     .Lif_chain_many_38
.Lif_chain_many_33:
.Lif_chain_many_34:
        jmp     .Lif_chain_many_36
.Lif_chain_many_29:
.Lif_chain_many_30:
        movl    $3, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 48(%rsp)
        addq    $52, %rsp
        jmp     .Lif_chain_many_38
.Lif_chain_many_end:
    .globl  if_else_if
    .type   if_else_if, @function
if_else_if:
        subq    $24, %rsp
.Lif_else_if_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_else_if_10
.Lif_else_if_7:
.Lif_else_if_8:
        movl    $2, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 20(%rsp)
.Lif_else_if_21:
        movl    20(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lif_else_if_10:
.Lif_else_if_11:
        movl    $3, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lif_else_if_16
.Lif_else_if_13:
.Lif_else_if_14:
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        jmp     .Lif_else_if_21
.Lif_else_if_16:
.Lif_else_if_17:
        movl    $5, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        addq    $24, %rsp
        jmp     .Lif_else_if_21
.Lif_else_if_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"