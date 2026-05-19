    .text
    .globl  scope
    .type   scope, @function
scope:
        subq    $40, %rsp
.Lscope_entry:
        movl    $0, 4(%rsp)
        movl    $1, 8(%rsp)
        movl    $2, 12(%rsp)
        movl    12(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    $4, 24(%rsp)
        movl    $5, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    8(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    $6, 36(%rsp)
        movl    $0, (%rsp)
.Lscope_17:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $40, %rsp
        ret
.Lscope_end:
    .globl  scope_reassign_variable
    .type   scope_reassign_variable, @function
scope_reassign_variable:
        subq    $24, %rsp
.Lscope_reassign_variable_entry:
        movl    $1, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $1, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    $3, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $4, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $5, %r11d
        movl    %r11d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lscope_reassign_variable_16:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $24, %rsp
        ret
.Lscope_reassign_variable_end:
    .globl  scope_reassign_in_parallel_scopes
    .type   scope_reassign_in_parallel_scopes, @function
scope_reassign_in_parallel_scopes:
        subq    $16, %rsp
.Lscope_reassign_in_parallel_scopes_entry:
        movl    $1, 4(%rsp)
        movl    $2, 8(%rsp)
        movl    $3, 12(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lscope_reassign_in_parallel_scopes_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lscope_reassign_in_parallel_scopes_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"