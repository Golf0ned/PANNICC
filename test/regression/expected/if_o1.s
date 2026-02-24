    .text
    .globl  if_basic
    .type   if_basic, @function
if_basic:
.Lif_basic_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lif_basic_2
.Lif_basic_0:
.Lif_basic_1:
        movl    $101, %r10d
.Lif_basic_3:
        movl    %r10d, %eax
        ret
.Lif_basic_2:
        movl    $1, %r10d
        jmp     .Lif_basic_3
.Lif_basic_end:
    .globl  if_basic_else
    .type   if_basic_else, @function
if_basic_else:
.Lif_basic_else_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lif_basic_else_2
.Lif_basic_else_0:
.Lif_basic_else_1:
        movl    $4, %r10d
.Lif_basic_else_4:
        movl    %r10d, %eax
        ret
.Lif_basic_else_2:
.Lif_basic_else_3:
        movl    $101, %r10d
        jmp     .Lif_basic_else_4
.Lif_basic_else_end:
    .globl  if_no_braces
    .type   if_no_braces, @function
if_no_braces:
.Lif_no_braces_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lif_no_braces_2
.Lif_no_braces_0:
.Lif_no_braces_1:
        movl    $1, %r10d
.Lif_no_braces_4:
        movl    %r10d, %eax
        ret
.Lif_no_braces_2:
.Lif_no_braces_3:
        movl    $1, %r10d
        jmp     .Lif_no_braces_4
.Lif_no_braces_end:
    .globl  if_chain
    .type   if_chain, @function
if_chain:
.Lif_chain_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lif_chain_4
.Lif_chain_0:
.Lif_chain_1:
        movq    $2, %r10
        cmpq    $0, %r10
        jne     .Lif_chain_5
.Lif_chain_2:
.Lif_chain_3:
        movl    $0, %r10d
.Lif_chain_7:
        movl    %r10d, %eax
        ret
.Lif_chain_4:
.Lif_chain_6:
        movl    $1, %r10d
        jmp     .Lif_chain_7
.Lif_chain_5:
        jmp     .Lif_chain_6
.Lif_chain_end:
    .globl  if_chain_many
    .type   if_chain_many, @function
if_chain_many:
.Lif_chain_many_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lif_chain_many_8
.Lif_chain_many_0:
.Lif_chain_many_1:
        movq    $2, %r10
        cmpq    $0, %r10
        jne     .Lif_chain_many_9
.Lif_chain_many_2:
.Lif_chain_many_3:
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lif_chain_many_6
.Lif_chain_many_4:
.Lif_chain_many_5:
        movl    $0, %r10d
.Lif_chain_many_11:
        movl    %r10d, %eax
        ret
.Lif_chain_many_8:
.Lif_chain_many_10:
        movl    $1, %r10d
        jmp     .Lif_chain_many_11
.Lif_chain_many_9:
        jmp     .Lif_chain_many_10
.Lif_chain_many_6:
.Lif_chain_many_7:
        movl    $3, %r10d
        jmp     .Lif_chain_many_11
.Lif_chain_many_end:
    .globl  if_else_if
    .type   if_else_if, @function
if_else_if:
.Lif_else_if_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lif_else_if_2
.Lif_else_if_0:
.Lif_else_if_1:
        movl    $2, %r10d
.Lif_else_if_8:
        movl    %r10d, %eax
        ret
.Lif_else_if_2:
.Lif_else_if_3:
        movq    $3, %r10
        cmpq    $0, %r10
        jne     .Lif_else_if_6
.Lif_else_if_4:
.Lif_else_if_5:
        movl    $4, %r10d
        jmp     .Lif_else_if_8
.Lif_else_if_6:
.Lif_else_if_7:
        movl    $5, %r10d
        jmp     .Lif_else_if_8
.Lif_else_if_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"