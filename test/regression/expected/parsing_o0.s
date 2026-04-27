    .text
    .globl  main
    .type   main, @function
main:
        subq    $4, %rsp
.Lmain_entry:
        movl    $0, (%rsp)
.Lmain_1:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Lmain_end:
    .globl  compound
    .type   compound, @function
compound:
        subq    $64, %rsp
.Lcompound_entry:
        movl    $9, %r10d
        imull   $123, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, 8(%rsp)
        movl    $123, %r10d
        subl    $122, %r10d
        movl    %r10d, 12(%rsp)
        movl    8(%rsp), %r11d
        movl    12(%rsp), %edi
        movl    %r11d, %r10d
        andl    %edi, %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $2, %r10d
        imull   $3, %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r11d
        movl    $1, %r10d
        addl    %r11d, %r10d
        movl    %r10d, 28(%rsp)
        movl    28(%rsp), %r11d
        movl    %r11d, %r10d
        subl    $4, %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, %r11d
        addl    $2, %r11d
        movl    %r11d, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $3, %r11d
        movl    %r11d, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $4, %r11d
        movl    %r11d, 44(%rsp)
        movl    44(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $5, %r11d
        movl    %r11d, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $6, %r11d
        movl    %r11d, 52(%rsp)
        movl    52(%rsp), %r10d
        movl    20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 56(%rsp)
        movl    56(%rsp), %r10d
        movl    %r10d, 60(%rsp)
.Lcompound_44:
        movl    60(%rsp), %r10d
        movl    %r10d, %eax
        addq    $64, %rsp
        ret
.Lcompound_end:
    .globl  parens
    .type   parens, @function
parens:
        subq    $16, %rsp
.Lparens_entry:
        movl    $2, %r10d
        subl    $1, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r11d
        movl    $3, %r10d
        subl    %r11d, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        leal    (,%r10d,4), %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lparens_10:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lparens_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"