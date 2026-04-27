    .text
    .globl  main
    .type   main, @function
main:
        subq    $32, %rsp
.Lmain_entry:
        movl    $67, %edi
        call    print
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $4, %edi
        call    alloc
        movq    %rax, %r10
        movq    %r10, 8(%rsp)
        movq    8(%rsp), %r10
        movq    %r10, 4(%rsp)
        movq    4(%rsp), %r10
        movq    %r10, %rdi
        call    dealloc
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    $0, 16(%rsp)
.Lmain_10:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"