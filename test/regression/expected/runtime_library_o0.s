    .text
    .globl  main
    .type   main, @function
main:
        subq    $48, %rsp
.Lmain_entry:
        movl    $67, (%rsp)
        movl    (%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    print
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    alloc
        addq    $16, %rsp
        movq    %rax, %r10
        movq    %r10, 16(%rsp)
        movq    16(%rsp), %r10
        movq    %r10, 8(%rsp)
        movq    8(%rsp), %r10
        movq    %r10, 20(%rsp)
        movq    20(%rsp), %r10
        movq    %r10, %rdi
        call    dealloc
        movl    %eax, %r10d
        movl    %r10d, 24(%rsp)
        movl    $0, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 32(%rsp)
.Lmain_18:
        movl    32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"