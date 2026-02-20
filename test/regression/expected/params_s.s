fun1:
        movl    %edi, %r11d
.fun1_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
.fun1_2:
        movl    %r10d, %eax
        ret
fun2:
        movl    %edi, %r10d
        movl    %esi, %r11d
.fun2_entry:
        subq    $16, %rsp
        call    fun1
        addq    $16, %rsp
        movl    %eax, %edi
        leal    (,%r10d,2), %esi
        movl    %esi, %r10d
        imull   %r11d, %r10d
        leal    (%edi,%r10d), %esi
        subq    $16, %rsp
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%esi,%r10d), %r11d
.fun2_8:
        movl    %r11d, %eax
        ret
fun3:
        movl    %edi, %esi
        movl    %esi, %edx
        movl    %edx, %ecx
        movl    %ecx, %edi
.fun3_entry:
        subq    $32, %rsp
        call    fun1
        addq    $32, %rsp
        movl    %eax, %r10d
        movl    %esi, %r11d
        imull   %r10d, %r11d
        movl    %edx, %r10d
        imull   %edi, %r10d
        leal    (%r11d,%r10d), %edi
        leal    (%edi,%ecx), %r10d
.fun3_9:
        movl    %r10d, %eax
        ret
call1:
.call1_entry:
        call    fun1
        movl    %eax, %r10d
.call1_1:
        movl    %r10d, %eax
        ret
call2:
        movl    %edi, %r11d
        movl    %esi, %r10d
.call2_entry:
        subq    $16, %rsp
        call    fun2
        addq    $16, %rsp
        movl    %eax, %r10d
.call2_3:
        movl    %r10d, %eax
        ret
call3:
        movl    %edi, %r11d
        movl    %esi, %r10d
.call3_entry:
        subq    $16, %rsp
        call    fun3
        addq    $16, %rsp
        movl    %eax, %r10d
.call3_3:
        movl    %r10d, %eax
        ret
call4:
.call4_entry:
        call    fun1
        movl    %eax, %r11d
        movl    %r11d, %r10d
        subl    $100, %r10d
.call4_2:
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
.super_fun_entry:
        leal    (%r10d,%r11d), %eax
        leal    (%eax,%edi), %r11d
        leal    (%r11d,%esi), %r10d
        leal    (%r10d,%edx), %r11d
        leal    (%r11d,%ecx), %r10d
        leal    (%r10d,%r8d), %r11d
        leal    (%r11d,%r9d), %r10d
.super_fun_15:
        movl    %r10d, %eax
        ret
call_super_fun:
        movl    %edi, %r11d
        movl    %esi, %r10d
.call_super_fun_entry:
        subq    $32, %rsp
        call    super_fun
        addq    $32, %rsp
        movl    %eax, %r10d
.call_super_fun_3:
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
.caller_saved_entry:
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
        call    super_fun
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
.caller_saved_26:
        movl    %r10d, %eax
        movq    (%rsp), %rbx
        movq    $8(%rsp), %r12
        movq    $16(%rsp), %r13
        movq    $24(%rsp), %r14
        movq    $32(%rsp), %r15
        addq    $48, %rsp
        ret