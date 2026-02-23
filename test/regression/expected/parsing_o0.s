main:
        subq    $4, %rsp
.Lmain_entry:
        movl    $0, (%rsp)
.Lmain_1:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
compound:
        subq    $76, %rsp
.Lcompound_entry:
        movl    $9, $4(%rsp)
        movl    $123, $8(%rsp)
        movl    $4(%rsp), %r11d
        movl    $8(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, $4(%rsp)
        movl    $3, $12(%rsp)
        movl    $4(%rsp), %r11d
        movl    $12(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, $4(%rsp)
        movl    $123, $16(%rsp)
        movl    $122, $20(%rsp)
        movl    $16(%rsp), %r11d
        movl    $20(%rsp), %edi
        movl    %r11d, %r10d
        subl    %edi, %r10d
        movl    %r10d, $16(%rsp)
        movl    $4(%rsp), %r11d
        movl    $16(%rsp), %edi
        movl    %r11d, %r10d
        andl    %edi, %r10d
        movl    %r10d, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, $28(%rsp)
        movl    $2, $32(%rsp)
        movl    $3, $36(%rsp)
        movl    $32(%rsp), %r11d
        movl    $36(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, $32(%rsp)
        movl    $28(%rsp), %r10d
        movl    $32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $28(%rsp)
        movl    $4, $40(%rsp)
        movl    $28(%rsp), %r11d
        movl    $40(%rsp), %edi
        movl    %r11d, %r10d
        subl    %edi, %r10d
        movl    %r10d, $28(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $44(%rsp)
        movl    $2, $48(%rsp)
        movl    $44(%rsp), %r10d
        movl    $48(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $44(%rsp)
        movl    $3, $52(%rsp)
        movl    $44(%rsp), %r10d
        movl    $52(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $44(%rsp)
        movl    $4, $56(%rsp)
        movl    $44(%rsp), %r10d
        movl    $56(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $44(%rsp)
        movl    $5, $60(%rsp)
        movl    $44(%rsp), %r10d
        movl    $60(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $44(%rsp)
        movl    $6, $64(%rsp)
        movl    $44(%rsp), %r10d
        movl    $64(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $44(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $68(%rsp)
        movl    $44(%rsp), %r10d
        movl    $68(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $44(%rsp)
        movl    $44(%rsp), %r10d
        movl    %r10d, $72(%rsp)
.Lcompound_63:
        movl    $72(%rsp), %r10d
        movl    %r10d, %eax
        addq    $76, %rsp
        ret
parens:
        subq    $20, %rsp
.Lparens_entry:
        movl    $4, (%rsp)
        movl    $3, $4(%rsp)
        movl    $2, $8(%rsp)
        movl    $1, $12(%rsp)
        movl    $8(%rsp), %r10d
        movl    $12(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $8(%rsp)
        movl    $4(%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $4(%rsp)
        movl    (%rsp), %r10d
        movl    $4(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
.Lparens_15:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret