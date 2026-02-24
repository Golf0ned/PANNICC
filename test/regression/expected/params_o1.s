    .text
    .globl  fun1
    .type   fun1, @function
fun1:
        movl    %edi, %r11d
.Lfun1_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
        movl    %r10d, %eax
        ret
.Lfun1_end:
    .globl  fun2
    .type   fun2, @function
fun2:
        movl    %edi, %r10d
        movl    %esi, %r11d
.Lfun2_entry:
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r10d, %edi
        call    fun1
        movq    (%rsp), %r11
        movq    8(%rsp), %r10
        addq    $16, %rsp
        movl    %eax, %edi
        leal    (,%r10d,2), %esi
        movl    %esi, %r10d
        imull   %r11d, %r10d
        leal    (%edi,%r10d), %esi
        subq    $16, %rsp
        movq    %rsi, 8(%rsp)
        movq    %r10, (%rsp)
        movl    %r11d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%esi,%r10d), %r11d
        movl    %r11d, %eax
        ret
.Lfun2_end:
    .globl  fun3
    .type   fun3, @function
fun3:
        movl    %edi, %esi
        movl    %esi, %edx
        movl    %edx, %ecx
        movl    %ecx, %edi
.Lfun3_entry:
        subq    $32, %rsp
        movq    %rdi, 24(%rsp)
        movq    %rsi, 16(%rsp)
        movq    %rdx, 8(%rsp)
        movq    %rcx, (%rsp)
        movl    %edi, %edi
        call    fun1
        movq    (%rsp), %rcx
        movq    8(%rsp), %rdx
        movq    16(%rsp), %rsi
        movq    24(%rsp), %rdi
        addq    $32, %rsp
        movl    %eax, %r10d
        movl    %esi, %r11d
        imull   %r10d, %r11d
        movl    %edx, %r10d
        imull   %edi, %r10d
        leal    (%r11d,%r10d), %edi
        leal    (%edi,%ecx), %r10d
        movl    %r10d, %eax
        ret
.Lfun3_end:
    .globl  call1
    .type   call1, @function
call1:
.Lcall1_entry:
        movl    $5, %edi
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
.Lcall1_end:
    .globl  call2
    .type   call2, @function
call2:
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall2_entry:
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r11d, %edi
        movl    %r10d, %esi
        call    fun2
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
.Lcall2_end:
    .globl  call3
    .type   call3, @function
call3:
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall3_entry:
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r11d, %edi
        movl    %r10d, %esi
        movl    $1, %edx
        movl    $0, %ecx
        call    fun3
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
.Lcall3_end:
    .globl  call4
    .type   call4, @function
call4:
.Lcall4_entry:
        movl    $10, %edi
        call    fun1
        movl    %eax, %r11d
        movl    %r11d, %r10d
        subl    $100, %r10d
        movl    %r10d, %eax
        ret
.Lcall4_end:
    .globl  super_fun
    .type   super_fun, @function
super_fun:
        movl    %edi, %r10d
        movl    %esi, %r11d
        movl    %edx, %edi
        movl    %ecx, %esi
        movl    %r8d, %edx
        movl    %r9d, %ecx
        movl    (%rsp), %r8d
        movl    8(%rsp), %r9d
.Lsuper_fun_entry:
        leal    (%r10d,%r11d), %eax
        leal    (%eax,%edi), %r11d
        leal    (%r11d,%esi), %r10d
        leal    (%r10d,%edx), %r11d
        leal    (%r11d,%ecx), %r10d
        leal    (%r10d,%r8d), %r11d
        leal    (%r11d,%r9d), %r10d
        movl    %r10d, %eax
        ret
.Lsuper_fun_end:
    .globl  call_super_fun
    .type   call_super_fun, @function
call_super_fun:
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall_super_fun_entry:
        subq    $32, %rsp
        movq    %r10, 24(%rsp)
        movq    %r11, 16(%rsp)
        movl    $1, %edi
        movl    $2, %esi
        movl    $3, %edx
        movl    $4, %ecx
        movl    $5, %r8d
        movl    $6, %r9d
        movl    %r11d, (%rsp)
        movl    %r10d, 8(%rsp)
        call    super_fun
        addq    $32, %rsp
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
.Lcall_super_fun_end:
    .globl  caller_saved
    .type   caller_saved, @function
caller_saved:
        subq    $48, %rsp
        movq    %r15, 32(%rsp)
        movq    %r14, 24(%rsp)
        movq    %r13, 16(%rsp)
        movq    %r12, 8(%rsp)
        movq    %rbx, (%rsp)
        movl    %edi, %ecx
        movl    %esi, %r14d
.Lcaller_saved_entry:
        movl    %ecx, %edx
        addl    $1, %edx
        movl    %ecx, %esi
        addl    $2, %esi
        movl    %ecx, %edi
        addl    $3, %edi
        movl    %ecx, %r11d
        addl    $4, %r11d
        movl    %ecx, %r10d
        addl    $5, %r10d
        movl    %ecx, %r8d
        addl    $6, %r8d
        movl    %ecx, %r9d
        addl    $7, %r9d
        movl    %ecx, %eax
        addl    $8, %eax
        movl    %ecx, %ebx
        addl    $9, %ebx
        movl    %ecx, %r12d
        addl    $10, %r12d
        movl    %ecx, %r13d
        addl    $11, %r13d
        subq    $80, %rsp
        movq    %rdi, 72(%rsp)
        movq    %rsi, 64(%rsp)
        movq    %rdx, 56(%rsp)
        movq    %r8, 48(%rsp)
        movq    %r9, 40(%rsp)
        movq    %r10, 32(%rsp)
        movq    %r11, 24(%rsp)
        movl    %edx, %edi
        movl    %esi, %esi
        movl    %edi, %edx
        movl    %r11d, %ecx
        movl    %r10d, %r8d
        movl    %r8d, %r9d
        movl    %r9d, (%rsp)
        movl    %eax, 8(%rsp)
        call    super_fun
        movq    24(%rsp), %r11
        movq    32(%rsp), %r10
        movq    40(%rsp), %r9
        movq    48(%rsp), %r8
        movq    56(%rsp), %rdx
        movq    64(%rsp), %rsi
        movq    72(%rsp), %rdi
        addq    $80, %rsp
        movl    %eax, %r15d
        leal    (%edx,%esi), %ecx
        leal    (%ecx,%edi), %esi
        leal    (%esi,%r11d), %edi
        leal    (%edi,%r10d), %r11d
        leal    (%r11d,%r8d), %edi
        leal    (%edi,%r9d), %r10d
        leal    (%r10d,%eax), %r11d
        leal    (%r11d,%ebx), %r10d
        leal    (%r10d,%r12d), %r11d
        leal    (%r11d,%r13d), %r10d
        leal    (%r10d,%r15d), %r11d
        leal    (%r11d,%r14d), %r10d
        movl    %r10d, %eax
        movq    (%rsp), %rbx
        movq    8(%rsp), %r12
        movq    16(%rsp), %r13
        movq    24(%rsp), %r14
        movq    32(%rsp), %r15
        addq    $48, %rsp
        ret
.Lcaller_saved_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"