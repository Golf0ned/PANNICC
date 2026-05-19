    .text
    .globl  lshift
    .type   lshift, @function
lshift:
        subq    $24, %rsp
.Llshift_entry:
        movl    $1, 4(%rsp)
        movl    $4, 8(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %ecx
        shll    $8, %ecx
        movl    %ecx, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r10d
        movl    8(%rsp), %r11d
        movl    %r10d, %ecx
        shll    %r11d, %ecx
        movl    %ecx, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, (%rsp)
.Llshift_13:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $24, %rsp
        ret
.Llshift_end:
    .globl  rshift
    .type   rshift, @function
rshift:
        subq    $24, %rsp
.Lrshift_entry:
        movl    $16, 4(%rsp)
        movl    $3, 8(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %ecx
        sarl    $8, %ecx
        movl    %ecx, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r10d
        movl    8(%rsp), %r11d
        movl    %r10d, %ecx
        sarl    %r11d, %ecx
        movl    %ecx, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lrshift_13:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $24, %rsp
        ret
.Lrshift_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"