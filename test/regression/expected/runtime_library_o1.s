    .text
    .globl  main
    .type   main, @function
main:
.Lmain_entry:
        movl    $67, %edi
        call    print
        movl    %eax, %r10d
        movl    $4, %edi
        call    alloc
        movq    %rax, %r10
        movq    %r10, %rdi
        call    dealloc
        movl    %eax, %r10d
        movl    $0, %eax
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"