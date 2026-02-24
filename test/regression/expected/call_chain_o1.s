    .text
    .globl  first
    .type   first, @function
first:
.Lfirst_entry:
        movl    $1, %eax
        ret
.Lfirst_end:
    .globl  second
    .type   second, @function
second:
.Lsecond_entry:
        call    first
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %r10d, %eax
        ret
.Lsecond_end:
    .globl  third
    .type   third, @function
third:
.Lthird_entry:
        call    second
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, %eax
        ret
.Lthird_end:
    .globl  fourth
    .type   fourth, @function
fourth:
.Lfourth_entry:
        call    third
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, %eax
        ret
.Lfourth_end:
    .globl  fifth
    .type   fifth, @function
fifth:
.Lfifth_entry:
        call    fourth
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $4, %r10d
        movl    %r10d, %eax
        ret
.Lfifth_end:
    .globl  sixth
    .type   sixth, @function
sixth:
.Lsixth_entry:
        call    fifth
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $5, %r10d
        movl    %r10d, %eax
        ret
.Lsixth_end:
    .globl  main
    .type   main, @function
main:
.Lmain_entry:
        call    sixth
        movl    %eax, %r10d
        call    first
        movl    %eax, %r10d
        movl    $0, %r11d
        addl    %r10d, %r11d
        subq    $16, %rsp
        movq    %r11, 8(%rsp)
        call    second
        movq    8(%rsp), %r11
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        call    third
        movq    8(%rsp), %rdi
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        call    fourth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movq    %r10, (%rsp)
        call    fifth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        call    sixth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movq    %r10, (%rsp)
        call    first
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        call    second
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movq    %r10, (%rsp)
        call    third
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        call    fourth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movq    %r10, (%rsp)
        call    fifth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %esi
        subq    $16, %rsp
        movq    %rsi, 8(%rsp)
        movq    %r11, (%rsp)
        call    sixth
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (%esi,%r11d), %r10d
        movl    %r10d, %eax
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"