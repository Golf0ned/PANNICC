    .text
    .globl  foo
    .type   foo, @function
foo:
.Lfoo_entry:
        movl    $0, %eax
        ret
.Lfoo_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"