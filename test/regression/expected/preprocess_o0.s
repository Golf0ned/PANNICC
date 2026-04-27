    .text
    .globl  deref
    .type   deref, @function
deref:
        subq    $12, %rsp
        movq    %rdi, %r10
.Lderef_entry:
        movq    %r10, 8(%rsp)
        movq    8(%rsp), %r11
        movl    (%r11), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lderef_7:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lderef_end:
    .globl  address
    .type   address, @function
address:
        subq    $12, %rsp
        movl    %edi, %r11d
.Laddress_entry:
        movl    %r11d, 8(%rsp)
        movq    %r10, (%rsp)
        movq    (%rsp), %r10
        movq    %r10, 4(%rsp)
.Laddress_5:
        movq    4(%rsp), %r10
        movq    %r10, %rax
        addq    $12, %rsp
        ret
.Laddress_end:
    .globl  address_deref
    .type   address_deref, @function
address_deref:
        subq    $16, %rsp
        movl    %edi, %r11d
.Laddress_deref_entry:
        movl    %r11d, 12(%rsp)
        movq    %r10, (%rsp)
        movq    (%rsp), %r10
        movl    (%r10), %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Laddress_deref_8:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Laddress_deref_end:
    .globl  address_deref_many
    .type   address_deref_many, @function
address_deref_many:
        subq    $44, %rsp
        movl    %edi, %esi
.Laddress_deref_many_entry:
        movl    %esi, 40(%rsp)
        movq    %r10, 4(%rsp)
        movq    4(%rsp), %r10
        movq    %r10, (%rsp)
        movq    %r11, 12(%rsp)
        movq    12(%rsp), %r10
        movq    %r10, 8(%rsp)
        movq    %rdi, 20(%rsp)
        movq    20(%rsp), %r10
        movq    %r10, 16(%rsp)
        movq    16(%rsp), %r11
        movq    (%r11), %r10
        movq    %r10, 24(%rsp)
        movq    24(%rsp), %r11
        movq    (%r11), %r10
        movq    %r10, 28(%rsp)
        movq    28(%rsp), %r11
        movl    (%r11), %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 36(%rsp)
.Laddress_deref_many_22:
        movl    36(%rsp), %r10d
        movl    %r10d, %eax
        addq    $44, %rsp
        ret
.Laddress_deref_many_end:
    .globl  return_ptr
    .type   return_ptr, @function
return_ptr:
        subq    $12, %rsp
.Lreturn_ptr_entry:
        movl    $1, (%rsp)
        movq    %r10, (%rsp)
        movq    (%rsp), %r10
        movq    %r10, 4(%rsp)
.Lreturn_ptr_4:
        movq    4(%rsp), %r10
        movq    %r10, %rax
        addq    $12, %rsp
        ret
.Lreturn_ptr_end:
    .globl  use_proto
    .type   use_proto, @function
use_proto:
        subq    $16, %rsp
.Luse_proto_entry:
        movl    $1, %edi
        call    foo
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $2, %edi
        call    bar
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r11d
        movl    (%rsp), %edi
        leal    (%r11d,%edi), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Luse_proto_10:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Luse_proto_end:
    .globl  main
    .type   main, @function
main:
        subq    $4, %rsp
.Lmain_entry:
        movl    $0, (%rsp)
.Lmain_1:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Lmain_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"