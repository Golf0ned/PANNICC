    .text
    .globl  main
    .type   main, @function
main:
.Lmain_entry:
        movl    $16384, %eax
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"