    .text
    .globl  fib_rec
    .type   fib_rec, @function
fib_rec:
        subq    $64, %rsp
        movl    %edi, %r10d
.Lfib_rec_entry:
        movl    %r10d, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lfib_rec_44
.Lfib_rec_16:
.Lfib_rec_17:
        movl    48(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    $1, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %edi
        movl    %r11d, %r10d
        subl    %edi, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lfib_rec_41
.Lfib_rec_23:
.Lfib_rec_24:
        movl    48(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    $1, 16(%rsp)
        movl    12(%rsp), %r11d
        movl    16(%rsp), %edi
        movl    %r11d, %r10d
        subl    %edi, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fib_rec
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 20(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    $2, 28(%rsp)
        movl    24(%rsp), %r10d
        movl    28(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 24(%rsp)
        movl    24(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fib_rec
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 32(%rsp)
        movl    20(%rsp), %r10d
        movl    32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 44(%rsp)
.Lfib_rec_47:
        movl    44(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lfib_rec_44:
.Lfib_rec_45:
        movl    $0, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 44(%rsp)
        jmp     .Lfib_rec_47
.Lfib_rec_41:
.Lfib_rec_42:
        movl    $1, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 44(%rsp)
        addq    $64, %rsp
        jmp     .Lfib_rec_47
.Lfib_rec_end:
    .globl  fib_it
    .type   fib_it, @function
fib_it:
        subq    $56, %rsp
        movl    %edi, %r10d
.Lfib_it_entry:
        movl    %r10d, 52(%rsp)
        movl    $0, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lfib_it_17:
        movl    52(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lfib_it_35
.Lfib_it_20:
.Lfib_it_21:
        movl    (%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    24(%rsp), %edi
        movl    28(%rsp), %r10d
        leal    (%edi,%r10d), %r11d
        movl    %r11d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    $1, 40(%rsp)
        movl    52(%rsp), %r10d
        movl    40(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 52(%rsp)
        jmp     .Lfib_it_17
.Lfib_it_35:
.Lfib_it_36:
        movl    (%rsp), %r10d
        movl    %r10d, 44(%rsp)
        movl    44(%rsp), %r10d
        movl    %r10d, 48(%rsp)
.Lfib_it_39:
        movl    48(%rsp), %r10d
        movl    %r10d, %eax
        addq    $56, %rsp
        ret
.Lfib_it_end:
    .globl  main
    .type   main, @function
main:
        subq    $32, %rsp
.Lmain_entry:
        movl    $10, (%rsp)
        movl    (%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fib_rec
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $1000, 4(%rsp)
        movl    4(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fib_it
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lmain_13:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"