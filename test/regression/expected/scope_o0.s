scope:
        subq    $80, %rsp
.scope_entry:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $2, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $3, $32(%rsp)
        movl    $28(%rsp), %r10d
        movl    $32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $28(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $4, $40(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        movl    $5, $48(%rsp)
        movl    $48(%rsp), %r10d
        movl    %r10d, $44(%rsp)
        movl    $44(%rsp), %r10d
        movl    %r10d, $52(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $56(%rsp)
        movl    $52(%rsp), %r10d
        movl    $56(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $52(%rsp)
        movl    $52(%rsp), %r10d
        movl    %r10d, $44(%rsp)
        movl    $6, $64(%rsp)
        movl    $64(%rsp), %r10d
        movl    %r10d, $60(%rsp)
        movl    $0, $68(%rsp)
        movl    $68(%rsp), %r10d
        movl    %r10d, $72(%rsp)
.scope_37:
        movl    $72(%rsp), %r10d
        movl    %r10d, %eax
        addq    $80, %rsp
        ret
scope_reassign_variable:
        subq    $48, %rsp
.scope_reassign_variable_entry:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $1, $12(%rsp)
        movl    $8(%rsp), %r10d
        movl    $12(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $3, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $4, $28(%rsp)
        movl    $24(%rsp), %r10d
        movl    $28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $32(%rsp)
        movl    $5, $36(%rsp)
        movl    $32(%rsp), %r10d
        movl    $36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $32(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $40(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $44(%rsp)
.scope_reassign_variable_31:
        movl    $44(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
scope_reassign_in_parallel_scopes:
        subq    $32, %rsp
.scope_reassign_in_parallel_scopes_entry:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $3, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $28(%rsp)
.scope_reassign_in_parallel_scopes_13:
        movl    $28(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret