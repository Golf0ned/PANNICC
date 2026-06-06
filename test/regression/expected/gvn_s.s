    .text
    .globl  basic
    .type   basic, @function
basic:
        movl    %edi, %edi
.Lbasic_entry:
        movl    %edi, %r10d
        addl    $1, %r10d
        leal    1(%edi,%r10d), %r11d
.Lbasic_4:
        movl    %r11d, %eax
        ret
.Lbasic_end:
    .globl  complex
    .type   complex, @function
complex:
        movl    %edi, %r10d
        movl    %esi, %edi
.Lcomplex_entry:
        movl    %r10d, %esi
        addl    $1, %esi
        movl    %r10d, %r11d
        addl    $1, %r11d
        movl    %edi, %r10d
        addl    $1, %r10d
        cmpl    $0, %r10d
        jne     .Lcomplex_7
.Lcomplex_5:
.Lcomplex_6:
.Lcomplex_8:
        leal    (%esi,%r11d), %edx
        leal    1(%edi,%edx), %r10d
.Lcomplex_12:
        movl    %r10d, %eax
        ret
.Lcomplex_7:
        jmp     .Lcomplex_8
.Lcomplex_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"