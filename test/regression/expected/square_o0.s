    .text
    .globl  square
    .type   square, @function
square:
        subq    $12, %rsp
        movl    %edi, %r10d
.Lsquare_entry:
        movl    %r10d, (%rsp)
        movl    (%rsp), %r11d
        movl    (%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lsquare_8:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lsquare_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"