    .text
    .globl  foo
    .type   foo, @function
foo:
        subq    $4, %rsp
.Lfoo_entry:
        movl    $0, (%rsp)
.Lfoo_1:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Lfoo_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"