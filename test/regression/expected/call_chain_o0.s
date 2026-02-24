    .text
    .globl  first
    .type   first, @function
first:
        subq    $8, %rsp
.Lfirst_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lfirst_3:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lfirst_end:
    .globl  second
    .type   second, @function
second:
        subq    $16, %rsp
.Lsecond_entry:
        call    first
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $1, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lsecond_8:
        movl    8(%rsp), %r10d
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
        movl    %r10d, (%rsp)
        movl    $2, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lthird_8:
        movl    8(%rsp), %r10d
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
        movl    %r10d, (%rsp)
        movl    $3, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lfourth_8:
        movl    8(%rsp), %r10d
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
        movl    %r10d, (%rsp)
        movl    $4, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lfifth_8:
        movl    8(%rsp), %r10d
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
        movl    %r10d, (%rsp)
        movl    $5, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lsixth_8:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lsixth_end:
    .globl  main
    .type   main, @function
main:
        subq    $96, %rsp
.Lmain_entry:
        call    sixth
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $0, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        call    first
        movl    %eax, %r10d
        movl    %r10d, 16(%rsp)
        movl    12(%rsp), %r11d
        movl    16(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        call    second
        movl    %eax, %r10d
        movl    %r10d, 24(%rsp)
        movl    20(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        call    third
        movl    %eax, %r10d
        movl    %r10d, 32(%rsp)
        movl    28(%rsp), %r10d
        movl    32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        call    fourth
        movl    %eax, %r10d
        movl    %r10d, 40(%rsp)
        movl    36(%rsp), %r10d
        movl    40(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 44(%rsp)
        call    fifth
        movl    %eax, %r10d
        movl    %r10d, 48(%rsp)
        movl    44(%rsp), %r10d
        movl    48(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 44(%rsp)
        movl    44(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 52(%rsp)
        call    sixth
        movl    %eax, %r10d
        movl    %r10d, 56(%rsp)
        movl    52(%rsp), %r10d
        movl    56(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 52(%rsp)
        movl    52(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 60(%rsp)
        call    first
        movl    %eax, %r10d
        movl    %r10d, 64(%rsp)
        movl    60(%rsp), %r10d
        movl    64(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        call    second
        movl    %eax, %r10d
        movl    %r10d, 68(%rsp)
        movl    60(%rsp), %r10d
        movl    68(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        call    third
        movl    %eax, %r10d
        movl    %r10d, 72(%rsp)
        movl    60(%rsp), %r10d
        movl    72(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        call    fourth
        movl    %eax, %r10d
        movl    %r10d, 76(%rsp)
        movl    60(%rsp), %r10d
        movl    76(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        call    fifth
        movl    %eax, %r10d
        movl    %r10d, 80(%rsp)
        movl    60(%rsp), %r10d
        movl    80(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        call    sixth
        movl    %eax, %r10d
        movl    %r10d, 84(%rsp)
        movl    60(%rsp), %r10d
        movl    84(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        movl    60(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 88(%rsp)
        movl    88(%rsp), %r10d
        movl    %r10d, 92(%rsp)
.Lmain_90:
        movl    92(%rsp), %r10d
        movl    %r10d, %eax
        addq    $96, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"