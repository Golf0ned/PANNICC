    .text
    .globl  lshift
    .type   lshift, @function
lshift:
.Llshift_entry:
        movl    $16, %eax
        ret
.Llshift_end:
    .globl  rshift
    .type   rshift, @function
rshift:
.Lrshift_entry:
        movl    $2, %eax
        ret
.Lrshift_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"