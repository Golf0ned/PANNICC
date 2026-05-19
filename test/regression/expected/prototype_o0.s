    .text
    .globl  use_proto
    .type   use_proto, @function
use_proto:
        subq    $16, %rsp
.Luse_proto_entry:
        movl    $1, %edi
        call    foo
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    $2, %edi
        call    bar
        movl    %eax, %r10d
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r10d
        movl    8(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
.Luse_proto_10:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Luse_proto_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"