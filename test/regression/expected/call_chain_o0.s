    .text
    .globl  first
    .type   first, @function
first:
        subq    $4, %rsp
.Lfirst_entry:
        movl    $1, (%rsp)
.Lfirst_1:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Lfirst_end:
    .globl  second
    .type   second, @function
second:
        subq    $16, %rsp
.Lsecond_entry:
        call    first
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $1, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lsecond_7:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lsecond_end:
    .globl  third
    .type   third, @function
third:
        subq    $16, %rsp
.Lthird_entry:
        call    second
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $2, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lthird_7:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lthird_end:
    .globl  fourth
    .type   fourth, @function
fourth:
        subq    $16, %rsp
.Lfourth_entry:
        call    third
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $3, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lfourth_7:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lfourth_end:
    .globl  fifth
    .type   fifth, @function
fifth:
        subq    $16, %rsp
.Lfifth_entry:
        call    fourth
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $4, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lfifth_7:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lfifth_end:
    .globl  sixth
    .type   sixth, @function
sixth:
        subq    $16, %rsp
.Lsixth_entry:
        call    fifth
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $5, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lsixth_7:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lsixth_end:
    .globl  main
    .type   main, @function
main:
        subq    $112, %rsp
.Lmain_entry:
        call    sixth
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    $0, 8(%rsp)
        call    first
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    8(%rsp), %edi
        movl    12(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        call    second
        movl    %eax, %r10d
        movl    %r10d, 20(%rsp)
        movl    8(%rsp), %edi
        movl    20(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        call    third
        movl    %eax, %r10d
        movl    %r10d, 28(%rsp)
        movl    8(%rsp), %r11d
        movl    28(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        call    fourth
        movl    %eax, %r10d
        movl    %r10d, 36(%rsp)
        movl    8(%rsp), %r11d
        movl    36(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        call    fifth
        movl    %eax, %r10d
        movl    %r10d, 44(%rsp)
        movl    8(%rsp), %r10d
        movl    44(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        call    sixth
        movl    %eax, %r10d
        movl    %r10d, 52(%rsp)
        movl    8(%rsp), %r10d
        movl    52(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 56(%rsp)
        movl    56(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        call    first
        movl    %eax, %r10d
        movl    %r10d, 60(%rsp)
        movl    8(%rsp), %r10d
        movl    60(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 64(%rsp)
        call    second
        movl    %eax, %r10d
        movl    %r10d, 68(%rsp)
        movl    64(%rsp), %r10d
        movl    68(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 72(%rsp)
        call    third
        movl    %eax, %r10d
        movl    %r10d, 76(%rsp)
        movl    72(%rsp), %r10d
        movl    76(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 80(%rsp)
        call    fourth
        movl    %eax, %r10d
        movl    %r10d, 84(%rsp)
        movl    80(%rsp), %r10d
        movl    84(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 88(%rsp)
        call    fifth
        movl    %eax, %r10d
        movl    %r10d, 92(%rsp)
        movl    88(%rsp), %r10d
        movl    92(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 96(%rsp)
        call    sixth
        movl    %eax, %r10d
        movl    %r10d, 100(%rsp)
        movl    96(%rsp), %r10d
        movl    100(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 104(%rsp)
        movl    104(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lmain_84:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $112, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"