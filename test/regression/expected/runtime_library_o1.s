    .text
    .globl  main
    .type   main, @function
main:
.Lmain_entry:
        movl    $4, %edi
        call    alloc
        movq    %rax, %r11
        movl    $67, (%r11)
        movl    (%r11), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    print
        addq    $16, %rsp
        movl    %eax, %r10d
        movq    %r11, %rdi
        call    dealloc
        movl    %eax, %r10d
        movl    $0, %eax
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"