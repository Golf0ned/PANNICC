fun1:
        movl    %edi, %r11d
.Lfun1_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
.Lfun1_2:
        movl    %r10d, %eax
        ret
fun2:
        movl    %edi, %r10d
        movl    %esi, %r11d
.Lfun2_entry:
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        movq    $8(%rsp), %rdi
        call    fun1
        movq    (%rsp), %r11d
        movq    $8(%rsp), %r10d
        addq    $16, %rsp
        movl    %eax, %edi
        leal    (,%r10d,2), %esi
        movl    %esi, %r10d
        imull   %r11d, %r10d
        leal    (%edi,%r10d), %esi
        subq    $16, %rsp
        movq    %esi, $8(%rsp)
        movq    %r10d, (%rsp)
        movq    %r11d, %rdi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%esi,%r10d), %r11d
.Lfun2_8:
        movl    %r11d, %eax
        ret
fun3:
        movl    %edi, %esi
        movl    %esi, %edx
        movl    %edx, %ecx
        movl    %ecx, %edi
.Lfun3_entry:
        subq    $32, %rsp
        movq    %edi, $24(%rsp)
        movq    %esi, $16(%rsp)
        movq    %edx, $8(%rsp)
        movq    %ecx, (%rsp)
        movq    $24(%rsp), %rdi
        call    fun1
        movq    (%rsp), %ecx
        movq    $8(%rsp), %edx
        movq    $16(%rsp), %esi
        movq    $24(%rsp), %edi
        addq    $32, %rsp
        movl    %eax, %r10d
        movl    %esi, %r11d
        imull   %r10d, %r11d
        movl    %edx, %r10d
        imull   %edi, %r10d
        leal    (%r11d,%r10d), %edi
        leal    (%edi,%ecx), %r10d
.Lfun3_9:
        movl    %r10d, %eax
        ret
call1:
.Lcall1_entry:
        movq    $5, %rdi
        call    fun1
        movl    %eax, %r10d
.Lcall1_1:
        movl    %r10d, %eax
        ret
call2:
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall2_entry:
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        movq    (%rsp), %rdi
        movq    $8(%rsp), %rsi
        call    fun2
        addq    $16, %rsp
        movl    %eax, %r10d
.Lcall2_3:
        movl    %r10d, %eax
        ret
call3:
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall3_entry:
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        movq    (%rsp), %rdi
        movq    $8(%rsp), %rsi
        movq    $1, %rdx
        movq    $0, %rcx
        call    fun3
        addq    $16, %rsp
        movl    %eax, %r10d
.Lcall3_3:
        movl    %r10d, %eax
        ret
call4:
.Lcall4_entry:
        movq    $10, %rdi
        call    fun1
        movl    %eax, %r11d
        movl    %r11d, %r10d
        subl    $100, %r10d
.Lcall4_2:
        movl    %r10d, %eax
        ret
super_fun:
        movl    %edi, %r10d
        movl    %esi, %r11d
        movl    %edx, %edi
        movl    %ecx, %esi
        movl    %r8d, %edx
        movl    %r9d, %ecx
        movl    (%rsp), %r8d
        movl    $8(%rsp), %r9d
.Lsuper_fun_entry:
        leal    (%r10d,%r11d), %eax
        leal    (%eax,%edi), %r11d
        leal    (%r11d,%esi), %r10d
        leal    (%r10d,%edx), %r11d
        leal    (%r11d,%ecx), %r10d
        leal    (%r10d,%r8d), %r11d
        leal    (%r11d,%r9d), %r10d
.Lsuper_fun_15:
        movl    %r10d, %eax
        ret
call_super_fun:
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall_super_fun_entry:
        subq    $32, %rsp
        movq    %r10d, $24(%rsp)
        movq    %r11d, $16(%rsp)
        movq    $1, %rdi
        movq    $2, %rsi
        movq    $3, %rdx
        movq    $4, %rcx
        movq    $5, %r8
        movq    $6, %r9
        movq    $16(%rsp), (%rsp)
        movq    $24(%rsp), $8(%rsp)
        call    super_fun
        addq    $32, %rsp
        movl    %eax, %r10d
.Lcall_super_fun_3:
        movl    %r10d, %eax
        ret
caller_saved:
        subq    $48, %rsp
        movq    %r15, $32(%rsp)
        movq    %r14, $24(%rsp)
        movq    %r13, $16(%rsp)
        movq    %r12, $8(%rsp)
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
        movq    %edi, $72(%rsp)
        movq    %esi, $64(%rsp)
        movq    %edx, $56(%rsp)
        movq    %r8d, $48(%rsp)
        movq    %r9d, $40(%rsp)
        movq    %r10d, $32(%rsp)
        movq    %r11d, $24(%rsp)
        movq    $56(%rsp), %rdi
        movq    $64(%rsp), %rsi
        movq    $72(%rsp), %rdx
        movq    $24(%rsp), %rcx
        movq    $32(%rsp), %r8
        movq    $48(%rsp), %r9
        movq    $40(%rsp), (%rsp)
        movq    %eax, $8(%rsp)
        call    super_fun
        movq    $24(%rsp), %r11d
        movq    $32(%rsp), %r10d
        movq    $40(%rsp), %r9d
        movq    $48(%rsp), %r8d
        movq    $56(%rsp), %edx
        movq    $64(%rsp), %esi
        movq    $72(%rsp), %edi
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
.Lcaller_saved_26:
        movl    %r10d, %eax
        movq    (%rsp), %rbx
        movq    $8(%rsp), %r12
        movq    $16(%rsp), %r13
        movq    $24(%rsp), %r14
        movq    $32(%rsp), %r15
        addq    $48, %rsp
        ret