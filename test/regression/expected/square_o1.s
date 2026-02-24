    .text
    .globl  square
    .type   square, @function
square:
        movl    %edi, %r11d
.Lsquare_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
        movl    %r10d, %eax
        ret
.Lsquare_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"