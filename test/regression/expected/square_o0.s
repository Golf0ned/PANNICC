square:
        subq    $16, %rsp
        movl    %edi, %r10d
.square_entry:
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    (%rsp), %r10d
        movl    $4(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
.square_11:
        movl    $8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret