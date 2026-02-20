fun1:
        movl    %edi, %r11d
.fun1_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
        movl    %r10d, %eax
        ret
fun2:
        movl    %edi, %r10d
        movl    %esi, %r11d
.fun2_entry:
        call    fun1
        movl    %eax, %edi
        leal    (,%r10d,2), %esi
        movl    %esi, %r10d
        imull   %r11d, %r10d
        leal    (%edi,%r10d), %esi
        call    fun1
        movl    %eax, %r10d
        leal    (%esi,%r10d), %r11d
        movl    %r11d, %eax
        ret
fun3:
        movl    %edi, %esi
        movl    %esi, %edx
        movl    %edx, %ecx
        movl    %ecx, %edi
.fun3_entry:
        call    fun1
        movl    %eax, %r10d
        movl    %esi, %r11d
        imull   %r10d, %r11d
        movl    %edx, %r10d
        imull   %edi, %r10d
        leal    (%r11d,%r10d), %edi
        leal    (%edi,%ecx), %r10d
        movl    %r10d, %eax
        ret
call1:
.call1_entry:
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
call2:
        movl    %edi, %r11d
        movl    %esi, %r10d
.call2_entry:
        call    fun2
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
call3:
        movl    %edi, %r11d
        movl    %esi, %r10d
.call3_entry:
        call    fun3
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
call4:
.call4_entry:
        call    fun1
        movl    %eax, %r11d
        movl    %r11d, %r10d
        subl    $100, %r10d
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
        movl    %r10d, %eax
        ret
call_super_fun:
        movl    %edi, %r11d
        movl    %esi, %r10d
.call_super_fun_entry:
        subq    $16, %rsp
        call    super_fun
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, %eax
        ret
caller_saved:
        movl    %edi, %r10d
        movl    %esi, %r10d
.caller_saved_entry:
        subq    $16, %rsp
        call    super_fun
        addq    $16, %rsp
        movl    %eax, %r11d
        movl    $23, %r10d
        addl    %r11d, %r10d
        movl    %r10d, %eax
        ret