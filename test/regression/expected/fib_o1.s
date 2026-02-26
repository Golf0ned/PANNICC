    .text
    .globl  fib_rec
    .type   fib_rec, @function
fib_rec:
        movl    %edi, %r11d
.Lfib_rec_entry:
        cmpl    $0, %r11d
        jne     .Lfib_rec_13
.Lfib_rec_1:
.Lfib_rec_2:
        movl    %r11d, %r10d
        subl    $1, %r10d
        cmpl    $0, %r10d
        jne     .Lfib_rec_11
.Lfib_rec_4:
.Lfib_rec_5:
        movl    %r11d, %r10d
        subl    $1, %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r10d, %edi
        call    fib_rec
        movq    (%rsp), %r11
        addq    $16, %rsp
        movl    %eax, %edi
        movl    %r11d, %r10d
        subl    $2, %r10d
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movq    %r10, (%rsp)
        movl    %r10d, %edi
        call    fib_rec
        movq    8(%rsp), %rdi
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        movl    %r11d, %r10d
.Lfib_rec_15:
        movl    %r10d, %eax
        ret
.Lfib_rec_13:
.Lfib_rec_14:
        movl    $0, %r10d
        jmp     .Lfib_rec_15
.Lfib_rec_11:
.Lfib_rec_12:
        movl    $1, %r10d
        jmp     .Lfib_rec_15
.Lfib_rec_end:
    .globl  fib_it
    .type   fib_it, @function
fib_it:
        movl    %edi, %r10d
.Lfib_it_entry:
        movl    $1, %r11d
        movl    %r10d, %edi
        movl    $0, %r10d
.Lfib_it_1:
        cmpl    $0, %edi
        jne     .Lfib_it_9
.Lfib_it_5:
.Lfib_it_6:
        leal    (%r10d,%r11d), %esi
        movl    %edi, %r10d
        subl    $1, %r10d
        movl    %r10d, %edi
        movl    %r11d, %r10d
        movl    %esi, %r11d
        jmp     .Lfib_it_1
.Lfib_it_9:
.Lfib_it_10:
        movl    %r10d, %eax
        ret
.Lfib_it_end:
    .globl  main
    .type   main, @function
main:
.Lmain_entry:
        movl    $10, %edi
        call    fib_rec
        movl    %eax, %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movl    $1000, %edi
        call    fib_it
        movq    8(%rsp), %rdi
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, %eax
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"