    .text
    .globl  use_proto
    .type   use_proto, @function
use_proto:
        subq    $32, %rsp
.Luse_proto_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    foo
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 4(%rsp)
        movl    $2, 8(%rsp)
        movl    8(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    bar
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r10d
        movl    12(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Luse_proto_13:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Luse_proto_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"