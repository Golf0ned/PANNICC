    .text
    .globl  main
    .type   main, @function
main:
        subq    $32, %rsp
.Lmain_entry:
        movl    $4, %edi
        call    alloc
        movq    %rax, %r10
        movq    %r10, 8(%rsp)
        movq    8(%rsp), %r10
        movq    %r10, 4(%rsp)
        movq    4(%rsp), %r10
        movl    $67, (%r10)
        movq    4(%rsp), %r11
        movl    (%r11), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    print
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 16(%rsp)
        movq    4(%rsp), %r10
        movq    %r10, %rdi
        call    dealloc
        movl    %eax, %r10d
        movl    %r10d, 20(%rsp)
        movl    $0, (%rsp)
.Lmain_15:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"