fun1:
        subq    $16, %rsp
        movl    %edi, %r10d
.fun1_entry:
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    (%rsp), %r10d
        movl    $4(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
.fun1_11:
        movl    $8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
fun2:
        subq    $48, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.fun2_entry:
        movl    %r11d, $32(%rsp)
        movl    %r10d, $36(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    $4(%rsp), %r10d
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, $8(%rsp)
        movl    $2, $12(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r11d
        movl    $12(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, $12(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $12(%rsp), %r11d
        movl    $16(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, $12(%rsp)
        movl    $8(%rsp), %r10d
        movl    $12(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $8(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $20(%rsp), %r10d
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, $24(%rsp)
        movl    $8(%rsp), %r10d
        movl    $24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $28(%rsp)
.fun2_33:
        movl    $28(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
fun3:
        subq    $48, %rsp
        movl    %edi, %r11d
        movl    %esi, %esi
        movl    %edx, %edi
        movl    %ecx, %r10d
.fun3_entry:
        movl    %r11d, $36(%rsp)
        movl    %esi, $32(%rsp)
        movl    %edi, $36(%rsp)
        movl    %r10d, $40(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, $16(%rsp)
        movl    $12(%rsp), %r10d
        movl    $16(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, $12(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $20(%rsp), %r10d
        movl    $24(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, $20(%rsp)
        movl    $12(%rsp), %r10d
        movl    $20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $12(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $12(%rsp), %r10d
        movl    $28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $32(%rsp)
.fun3_35:
        movl    $32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
call1:
        subq    $16, %rsp
.call1_entry:
        movl    $5, (%rsp)
        movl    (%rsp), %r10d
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $4(%rsp)
.call1_6:
        movl    $4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
call2:
        subq    $32, %rsp
        movl    %edi, %r10d
        movl    %esi, %r11d
.call2_entry:
        movl    %r10d, $16(%rsp)
        movl    %r11d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $4(%rsp), %r10d
        movl    $8(%rsp), %r11d
        call    fun2
        movl    %eax, %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $12(%rsp)
.call2_14:
        movl    $12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
call3:
        subq    $32, %rsp
        movl    %edi, %r10d
        movl    %esi, %r11d
.call3_entry:
        movl    %r10d, $24(%rsp)
        movl    %r11d, $28(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $1, $12(%rsp)
        movl    $0, $12(%rsp)
        movl    $4(%rsp), %r10d
        movl    $8(%rsp), %r11d
        movl    $12(%rsp), %edi
        movl    $12(%rsp), %esi
        call    fun3
        movl    %eax, %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $20(%rsp)
.call3_18:
        movl    $20(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
call4:
        subq    $16, %rsp
.call4_entry:
        movl    $10, (%rsp)
        movl    (%rsp), %r10d
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $100, $4(%rsp)
        movl    (%rsp), %r10d
        movl    $4(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $8(%rsp)
.call4_10:
        movl    $8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
super_fun:
        subq    $68, %rsp
        movl    %edi, %esi
        movl    %esi, %edx
        movl    %edx, %ecx
        movl    %ecx, %r8d
        movl    %r8d, %r9d
        movl    %r9d, %edi
        movl    $68(%rsp), %r11d
        movl    $76(%rsp), %r10d
.super_fun_entry:
        movl    %esi, $36(%rsp)
        movl    %edx, $40(%rsp)
        movl    %ecx, $44(%rsp)
        movl    %r8d, $48(%rsp)
        movl    %r9d, $52(%rsp)
        movl    %edi, $56(%rsp)
        movl    %r11d, $60(%rsp)
        movl    %r10d, $64(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $24(%rsp), %r11d
        movl    $28(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, $24(%rsp)
        movl    $44(%rsp), %r10d
        movl    %r10d, $32(%rsp)
        movl    $24(%rsp), %r10d
        movl    $32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $48(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        movl    $24(%rsp), %r11d
        movl    $36(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, $24(%rsp)
        movl    $52(%rsp), %r10d
        movl    %r10d, $32(%rsp)
        movl    $24(%rsp), %r10d
        movl    $32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $56(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        movl    $24(%rsp), %r10d
        movl    $36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $60(%rsp), %r10d
        movl    %r10d, $40(%rsp)
        movl    $24(%rsp), %r10d
        movl    $40(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $64(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $24(%rsp), %r10d
        movl    $28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $32(%rsp)
.super_fun_55:
        movl    $32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $68, %rsp
        ret
call_super_fun:
        subq    $48, %rsp
        movl    %edi, %r10d
        movl    %esi, %r11d
.call_super_fun_entry:
        movl    %r10d, $32(%rsp)
        movl    %r11d, $36(%rsp)
        movl    $1, $4(%rsp)
        movl    $2, $8(%rsp)
        movl    $3, $12(%rsp)
        movl    $4, $12(%rsp)
        movl    $5, $16(%rsp)
        movl    $6, $20(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $4(%rsp), %r10d
        movl    $8(%rsp), %r11d
        movl    $12(%rsp), %edi
        movl    $12(%rsp), %esi
        movl    $16(%rsp), %edx
        movl    $20(%rsp), %ecx
        movl    $24(%rsp), %r8d
        movl    $28(%rsp), %r9d
        call    super_fun
        movl    %eax, %r10d
        movl    %r10d, $32(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $36(%rsp)
.call_super_fun_26:
        movl    $36(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
call_many:
        subq    $80, %rsp
        movl    %edi, %r10d
        movl    %esi, %r11d
.call_many_entry:
        movl    %r10d, $36(%rsp)
        movl    %r11d, $40(%rsp)
        movl    $5, $8(%rsp)
        movl    $8(%rsp), %r10d
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %r11d
        call    call_super_fun
        movl    %eax, %r10d
        movl    %r10d, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $32(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        movl    $32(%rsp), %r10d
        movl    $36(%rsp), %r11d
        call    call2
        movl    %eax, %r10d
        movl    %r10d, $32(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $40(%rsp)
        movl    $36(%rsp), %r10d
        movl    $40(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $36(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $36(%rsp), %r10d
        movl    $28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $36(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $32(%rsp)
.call_many_44:
        movl    $32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $80, %rsp
        ret