    .text
    .globl  deref
    .type   deref, @function
deref:
        movq    %rdi, %r11
.Lderef_entry:
        movl    (%r11), %r10d
        movl    %r10d, %eax
        ret
.Lderef_end:
    .globl  address
    .type   address, @function
address:
        subq    $4, %rsp
        movl    %edi, %r10d
.Laddress_entry:
        movl    %r10d, (%rsp)
        movq    %rsp, %rax
        addq    $4, %rsp
        ret
.Laddress_end:
    .globl  address_deref
    .type   address_deref, @function
address_deref:
        subq    $4, %rsp
        movl    %edi, %r10d
.Laddress_deref_entry:
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Laddress_deref_end:
    .globl  address_deref_many
    .type   address_deref_many, @function
address_deref_many:
        subq    $12, %rsp
        movl    %edi, %edi
.Laddress_deref_many_entry:
        movl    %edi, (%rsp)
        movq    %r10, 4(%rsp)
        movq    %r11, 8(%rsp)
        movq    8(%rsp), %r10
        movq    (%r10), %r11
        movl    (%r11), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Laddress_deref_many_end:
    .globl  return_ptr
    .type   return_ptr, @function
return_ptr:
        subq    $4, %rsp
.Lreturn_ptr_entry:
        movl    $1, (%rsp)
        movq    %rsp, %rax
        addq    $4, %rsp
        ret
.Lreturn_ptr_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"