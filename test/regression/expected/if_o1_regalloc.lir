if_basic:  # 0 params, 0 stack bytes
.if_basic_entry:
        cmpl    $1, $0
        jne     .if_basic_2
.if_basic_0:
.if_basic_1:
        movl    $101, %r10d
.if_basic_3:
        movl    %r10d, %eax
        ret
.if_basic_2:
        movl    $1, %r10d
        jmp     .if_basic_3

if_basic_else:  # 0 params, 0 stack bytes
.if_basic_else_entry:
        cmpl    $1, $0
        jne     .if_basic_else_2
.if_basic_else_0:
.if_basic_else_1:
        movl    $4, %r10d
.if_basic_else_4:
        movl    %r10d, %eax
        ret
.if_basic_else_2:
.if_basic_else_3:
        movl    $101, %r10d
        jmp     .if_basic_else_4

if_no_braces:  # 0 params, 0 stack bytes
.if_no_braces_entry:
        cmpl    $1, $0
        jne     .if_no_braces_2
.if_no_braces_0:
.if_no_braces_1:
        movl    $1, %r10d
.if_no_braces_4:
        movl    %r10d, %eax
        ret
.if_no_braces_2:
.if_no_braces_3:
        movl    $1, %r10d
        jmp     .if_no_braces_4

if_chain:  # 0 params, 0 stack bytes
.if_chain_entry:
        cmpl    $1, $0
        jne     .if_chain_4
.if_chain_0:
.if_chain_1:
        cmpl    $2, $0
        jne     .if_chain_5
.if_chain_2:
.if_chain_3:
        movl    $0, %r10d
.if_chain_7:
        movl    %r10d, %eax
        ret
.if_chain_4:
.if_chain_6:
        movl    $1, %r10d
        jmp     .if_chain_7
.if_chain_5:
        jmp     .if_chain_6

if_chain_many:  # 0 params, 0 stack bytes
.if_chain_many_entry:
        cmpl    $1, $0
        jne     .if_chain_many_8
.if_chain_many_0:
.if_chain_many_1:
        cmpl    $2, $0
        jne     .if_chain_many_9
.if_chain_many_2:
.if_chain_many_3:
        cmpl    $0, $0
        jne     .if_chain_many_6
.if_chain_many_4:
.if_chain_many_5:
        movl    $0, %r10d
.if_chain_many_11:
        movl    %r10d, %eax
        ret
.if_chain_many_8:
.if_chain_many_10:
        movl    $1, %r10d
        jmp     .if_chain_many_11
.if_chain_many_9:
        jmp     .if_chain_many_10
.if_chain_many_6:
.if_chain_many_7:
        movl    $3, %r10d
        jmp     .if_chain_many_11

if_else_if:  # 0 params, 0 stack bytes
.if_else_if_entry:
        cmpl    $1, $0
        jne     .if_else_if_2
.if_else_if_0:
.if_else_if_1:
        movl    $2, %r10d
.if_else_if_8:
        movl    %r10d, %eax
        ret
.if_else_if_2:
.if_else_if_3:
        cmpl    $3, $0
        jne     .if_else_if_6
.if_else_if_4:
.if_else_if_5:
        movl    $4, %r10d
        jmp     .if_else_if_8
.if_else_if_6:
.if_else_if_7:
        movl    $5, %r10d
        jmp     .if_else_if_8