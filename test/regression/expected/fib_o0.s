    .text
    .globl  fib_rec
    .type   fib_rec, @function
fib_rec:
        subq    $32, %rsp
        movl    %edi, %r10d
.Lfib_rec_entry:
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lfib_rec_33
.Lfib_rec_10:
.Lfib_rec_11:
        movl    (%rsp), %r11d
        movl    %r11d, %r10d
        subl    $1, %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lfib_rec_30
.Lfib_rec_15:
.Lfib_rec_16:
        movl    (%rsp), %r11d
        movl    %r11d, %r10d
        subl    $1, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fib_rec
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 16(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, %r11d
        subl    $2, %r11d
        movl    %r11d, 20(%rsp)
        movl    20(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fib_rec
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 24(%rsp)
        movl    16(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lfib_rec_35:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lfib_rec_33:
.Lfib_rec_34:
        movl    $0, 4(%rsp)
        jmp     .Lfib_rec_35
.Lfib_rec_30:
.Lfib_rec_31:
        movl    $1, 4(%rsp)
        addq    $32, %rsp
        jmp     .Lfib_rec_35
.Lfib_rec_end:
    .globl  fib_it
    .type   fib_it, @function
fib_it:
        subq    $24, %rsp
        movl    %edi, %r10d
.Lfib_it_entry:
        movl    %r10d, (%rsp)
        movl    $0, 8(%rsp)
        movl    $1, 12(%rsp)
.Lfib_it_7:
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lfib_it_19
.Lfib_it_9:
.Lfib_it_10:
        movl    8(%rsp), %edi
        movl    12(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, %r11d
        subl    $1, %r11d
        movl    %r11d, (%rsp)
        jmp     .Lfib_it_7
.Lfib_it_19:
.Lfib_it_20:
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lfib_it_22:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $24, %rsp
        ret
.Lfib_it_end:
    .globl  main
    .type   main, @function
main:
        subq    $16, %rsp
.Lmain_entry:
        movl    $10, %edi
        call    fib_rec
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $1000, %edi
        call    fib_it
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %edi
        leal    (%r10d,%edi), %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lmain_10:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"