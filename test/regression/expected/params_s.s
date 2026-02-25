    .text
    .globl  fun1
    .type   fun1, @function
fun1:
        movl    %edi, %r11d
.Lfun1_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
.Lfun1_2:
        movl    %r10d, %eax
        ret
.Lfun1_end:
    .globl  fun2
    .type   fun2, @function
fun2:
        movl    %edi, %r10d
        movl    %esi, %esi
.Lfun2_entry:
        subq    $16, %rsp
        movq    %rsi, 8(%rsp)
        movq    %r10, (%rsp)
        movl    %r10d, %edi
        call    fun1
        movq    (%rsp), %r10
        movq    8(%rsp), %rsi
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (,%r10d,2), %edi
        movl    %edi, %r10d
        imull   %esi, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %rdi, 8(%rsp)
        movq    %rsi, (%rsp)
        movl    %esi, %edi
        call    fun1
        movq    8(%rsp), %rdi
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (%edi,%r11d), %r10d
.Lfun2_8:
        movl    %r10d, %eax
        ret
.Lfun2_end:
    .globl  fun3
    .type   fun3, @function
fun3:
        movl    %edi, %ecx
        movl    %esi, %esi
        movl    %edx, %edx
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
        movl    %ecx, %r11d
        imull   %r10d, %r11d
        movl    %esi, %r10d
        imull   %edi, %r10d
        leal    (%r11d,%r10d), %edi
        leal    (%edi,%edx), %r10d
.Lfun3_9:
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
.Lcall1_1:
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
.Lcall2_3:
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
.Lcall3_3:
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
.Lcall4_2:
        movl    %r10d, %eax
        ret
.Lcall4_end:
    .globl  super_fun
    .type   super_fun, @function
super_fun:
        movl    %edi, %eax
        movl    %esi, %r9d
        movl    %edx, %ecx
        movl    %ecx, %r11d
        movl    %r8d, %edi
        movl    %r9d, %esi
        movl    (%rsp), %edx
        movl    8(%rsp), %r8d
.Lsuper_fun_entry:
        leal    (%eax,%r9d), %r10d
        leal    (%r10d,%ecx), %r9d
        leal    (%r9d,%r11d), %r10d
        leal    (%r10d,%edi), %r11d
        leal    (%r11d,%esi), %r10d
        leal    (%r10d,%edx), %r11d
        leal    (%r11d,%r8d), %r10d
.Lsuper_fun_15:
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
.Lcall_super_fun_3:
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
        movl    %edi, %r12d
        movl    %esi, %r13d
.Lcaller_saved_entry:
        movl    %r12d, %r14d
        addl    $1, %r14d
        movl    %r12d, %r8d
        addl    $2, %r8d
        movl    %r12d, %ebx
        addl    $3, %ebx
        movl    %r12d, %eax
        addl    $4, %eax
        movl    %r12d, %r9d
        addl    $5, %r9d
        movl    %r12d, %edx
        addl    $6, %edx
        movl    %r12d, %ecx
        addl    $7, %ecx
        movl    %r12d, %esi
        addl    $8, %esi
        movl    %r12d, %edi
        addl    $9, %edi
        movl    %r12d, %r10d
        addl    $10, %r10d
        movl    %r12d, %r11d
        addl    $11, %r11d
        subq    $80, %rsp
        movq    %rdi, 72(%rsp)
        movq    %rsi, 64(%rsp)
        movq    %rdx, 56(%rsp)
        movq    %rcx, 48(%rsp)
        movq    %r8, 40(%rsp)
        movq    %r9, 32(%rsp)
        movq    %r10, 24(%rsp)
        movq    %r11, 16(%rsp)
        movl    %r14d, %edi
        movl    %r8d, %esi
        movl    %ebx, %edx
        movl    %eax, %ecx
        movl    %r9d, %r8d
        movl    %edx, %r9d
        movl    %ecx, (%rsp)
        movl    %esi, 8(%rsp)
        call    super_fun
        movq    16(%rsp), %r11
        movq    24(%rsp), %r10
        movq    32(%rsp), %r9
        movq    40(%rsp), %r8
        movq    48(%rsp), %rcx
        movq    56(%rsp), %rdx
        movq    64(%rsp), %rsi
        movq    72(%rsp), %rdi
        addq    $80, %rsp
        movl    %eax, %r12d
        leal    (%r14d,%r8d), %r15d
        leal    (%r15d,%ebx), %r8d
        leal    (%r8d,%eax), %ebx
        leal    (%ebx,%r9d), %r8d
        leal    (%r8d,%edx), %r9d
        leal    (%r9d,%ecx), %edx
        leal    (%edx,%esi), %ecx
        leal    (%ecx,%edi), %esi
        leal    (%esi,%r10d), %edi
        leal    (%edi,%r11d), %r10d
        leal    (%r10d,%r12d), %r11d
        leal    (%r11d,%r13d), %r10d
.Lcaller_saved_26:
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