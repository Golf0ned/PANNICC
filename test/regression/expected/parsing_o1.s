    .text
    .globl  main
    .type   main, @function
main:
.Lmain_entry:
        movl    $0, %eax
        ret
.Lmain_end:
    .globl  compound
    .type   compound, @function
compound:
.Lcompound_entry:
        movl    $23, %eax
        ret
.Lcompound_end:
    .globl  parens
    .type   parens, @function
parens:
.Lparens_entry:
        movl    $8, %eax
        ret
.Lparens_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"