    .text
    .globl  use_proto
    .type   use_proto, @function
use_proto:
.Luse_proto_entry:
        movl    $1, %edi
        call    foo
        movl    %eax, %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movl    $2, %edi
        call    bar
        movq    8(%rsp), %rdi
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, %eax
        ret
.Luse_proto_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"