    .text
    .globl  deref
    .type   deref, @function
deref:
        subq    $16, %rsp
        movq    %rdi, %r10
.Lderef_entry:
        movq    %r10, 12(%rsp)
        movq    12(%rsp), %r10
        movq    %r10, (%rsp)
        movq    (%rsp), %r11
        movl    (%r11), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lderef_9:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lderef_end:
    .globl  address
    .type   address, @function
address:
        subq    $16, %rsp
        movl    %edi, %r10d
.Laddress_entry:
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movq    %r11, 4(%rsp)
        movq    4(%rsp), %r10
        movq    %r10, 8(%rsp)
.Laddress_7:
        movq    8(%rsp), %r10
        movq    %r10, %rax
        addq    $16, %rsp
        ret
.Laddress_end:
    .globl  address_deref
    .type   address_deref, @function
address_deref:
        subq    $20, %rsp
        movl    %edi, %r10d
.Laddress_deref_entry:
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, (%rsp)
        movq    %r11, 4(%rsp)
        movq    4(%rsp), %r10
        movl    (%r10), %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Laddress_deref_10:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Laddress_deref_end:
    .globl  address_deref_many
    .type   address_deref_many, @function
address_deref_many:
        subq    $60, %rsp
        movl    %edi, %r10d
.Laddress_deref_many_entry:
        movl    %r10d, 56(%rsp)
        movl    56(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movq    %r11, 8(%rsp)
        movq    8(%rsp), %r10
        movq    %r10, (%rsp)
        movq    (%rsp), %r10
        movq    %r10, 16(%rsp)
        movq    %rdi, 20(%rsp)
        movq    20(%rsp), %r10
        movq    %r10, 12(%rsp)
        movq    12(%rsp), %r10
        movq    %r10, 28(%rsp)
        movq    %rsi, 32(%rsp)
        movq    32(%rsp), %r10
        movq    %r10, 24(%rsp)
        movq    24(%rsp), %r10
        movq    %r10, 36(%rsp)
        movq    36(%rsp), %r11
        movq    (%r11), %r10
        movq    %r10, 40(%rsp)
        movq    40(%rsp), %r11
        movq    (%r11), %r10
        movq    %r10, 44(%rsp)
        movq    44(%rsp), %r11
        movl    (%r11), %r10d
        movl    %r10d, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 52(%rsp)
.Laddress_deref_many_30:
        movl    52(%rsp), %r10d
        movl    %r10d, %eax
        addq    $60, %rsp
        ret
.Laddress_deref_many_end:
    .globl  return_ptr
    .type   return_ptr, @function
return_ptr:
        subq    $20, %rsp
.Lreturn_ptr_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movq    %r11, 8(%rsp)
        movq    8(%rsp), %r10
        movq    %r10, 12(%rsp)
.Lreturn_ptr_8:
        movq    12(%rsp), %r10
        movq    %r10, %rax
        addq    $20, %rsp
        ret
.Lreturn_ptr_end:
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
        movl    %r10d, (%rsp)
        movl    $2, 4(%rsp)
        movl    4(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    bar
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    8(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Luse_proto_13:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Luse_proto_end:
    .globl  main
    .type   main, @function
main:
        subq    $8, %rsp
.Lmain_entry:
        movl    $0, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, (%rsp)
.Lmain_3:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"