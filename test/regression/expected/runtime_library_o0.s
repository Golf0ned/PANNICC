    .text
    .globl  main
    .type   main, @function
main:
        subq    $16, %rsp
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
        movl    $0, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lmain_7:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"