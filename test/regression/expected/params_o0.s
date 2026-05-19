    .text
    .globl  fun1
    .type   fun1, @function
fun1:
        subq    $12, %rsp
        movl    %edi, %r10d
.Lfun1_entry:
        movl    %r10d, (%rsp)
        movl    (%rsp), %r11d
        movl    (%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lfun1_8:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lfun1_end:
    .globl  fun2
    .type   fun2, @function
fun2:
        subq    $48, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lfun2_entry:
        movl    %r11d, 4(%rsp)
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r11d
        leal    (,%r11d,2), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r11d
        movl    8(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 20(%rsp)
        movl    12(%rsp), %r10d
        movl    20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 24(%rsp)
        movl    8(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 28(%rsp)
        movl    24(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lfun2_27:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
.Lfun2_end:
    .globl  fun3
    .type   fun3, @function
fun3:
        subq    $48, %rsp
        movl    %edi, %esi
        movl    %esi, %r11d
        movl    %edx, %edi
        movl    %ecx, %r10d
.Lfun3_entry:
        movl    %esi, 8(%rsp)
        movl    %r11d, 12(%rsp)
        movl    %edi, 16(%rsp)
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 28(%rsp)
        movl    8(%rsp), %edi
        movl    28(%rsp), %r11d
        movl    %edi, %r10d
        imull   %r11d, %r10d
        movl    %r10d, 32(%rsp)
        movl    12(%rsp), %r11d
        movl    20(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 28(%rsp)
        movl    32(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    16(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 36(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lfun3_29:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
.Lfun3_end:
    .globl  call1
    .type   call1, @function
call1:
        subq    $16, %rsp
.Lcall1_entry:
        movl    $5, %edi
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, (%rsp)
.Lcall1_4:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall1_end:
    .globl  call2
    .type   call2, @function
call2:
        subq    $16, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall2_entry:
        movl    %r11d, 4(%rsp)
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r11d, %edi
        movl    %r10d, %esi
        call    fun2
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lcall2_10:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall2_end:
    .globl  call3
    .type   call3, @function
call3:
        subq    $16, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall3_entry:
        movl    %r11d, 4(%rsp)
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
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
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lcall3_10:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall3_end:
    .globl  call4
    .type   call4, @function
call4:
        subq    $16, %rsp
.Lcall4_entry:
        movl    $10, %edi
        call    fun1
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r11d
        movl    %r11d, %r10d
        subl    $100, %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
.Lcall4_7:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall4_end:
    .globl  super_fun
    .type   super_fun, @function
super_fun:
        subq    $64, %rsp
        movl    %edi, %r9d
        movl    %esi, %edi
        movl    %edx, %edx
        movl    %ecx, %r8d
        movl    %r8d, %ecx
        movl    %r9d, %esi
        movl    64(%rsp), %r11d
        movl    72(%rsp), %r10d
.Lsuper_fun_entry:
        movl    %r9d, 24(%rsp)
        movl    %edi, 28(%rsp)
        movl    %edx, 32(%rsp)
        movl    %r8d, 28(%rsp)
        movl    %ecx, 32(%rsp)
        movl    %esi, 36(%rsp)
        movl    %r11d, 24(%rsp)
        movl    %r10d, 28(%rsp)
        movl    24(%rsp), %edi
        movl    28(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 36(%rsp)
        movl    36(%rsp), %r11d
        movl    32(%rsp), %edi
        leal    (%r11d,%edi), %r10d
        movl    %r10d, 40(%rsp)
        movl    40(%rsp), %r11d
        movl    28(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 44(%rsp)
        movl    44(%rsp), %r10d
        movl    32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 52(%rsp)
        movl    52(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 56(%rsp)
        movl    56(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 60(%rsp)
        movl    60(%rsp), %r10d
        movl    %r10d, 32(%rsp)
.Lsuper_fun_46:
        movl    32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $64, %rsp
        ret
.Lsuper_fun_end:
    .globl  call_super_fun
    .type   call_super_fun, @function
call_super_fun:
        subq    $16, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall_super_fun_entry:
        movl    %r11d, 4(%rsp)
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
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
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lcall_super_fun_10:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall_super_fun_end:
    .globl  caller_saved
    .type   caller_saved, @function
caller_saved:
        subq    $160, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcaller_saved_entry:
        movl    %r11d, 4(%rsp)
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $4, %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $5, %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $6, %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $7, %r10d
        movl    %r10d, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $8, %r10d
        movl    %r10d, 48(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 44(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $9, %r10d
        movl    %r10d, 56(%rsp)
        movl    56(%rsp), %r10d
        movl    %r10d, 52(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $10, %r10d
        movl    %r10d, 88(%rsp)
        movl    88(%rsp), %r10d
        movl    %r10d, 60(%rsp)
        movl    4(%rsp), %r11d
        movl    %r11d, %r10d
        addl    $11, %r10d
        movl    %r10d, 96(%rsp)
        movl    96(%rsp), %r10d
        movl    %r10d, 92(%rsp)
        movl    12(%rsp), %r10d
        movl    20(%rsp), %r11d
        movl    28(%rsp), %edi
        movl    28(%rsp), %esi
        movl    36(%rsp), %edx
        movl    28(%rsp), %ecx
        movl    36(%rsp), %r8d
        movl    44(%rsp), %r9d
        subq    $80, %rsp
        movq    %rdi, 72(%rsp)
        movq    %rsi, 64(%rsp)
        movq    %rdx, 56(%rsp)
        movq    %rcx, 48(%rsp)
        movq    %r8, 40(%rsp)
        movq    %r9, 32(%rsp)
        movq    %r10, 24(%rsp)
        movq    %r11, 16(%rsp)
        movl    %r10d, %edi
        movl    %r11d, %esi
        movl    %edi, %edx
        movl    %esi, %ecx
        movl    %edx, %r8d
        movl    %ecx, %r9d
        movl    %r8d, (%rsp)
        movl    %r9d, 8(%rsp)
        call    super_fun
        addq    $80, %rsp
        movl    %eax, %r10d
        movl    %r10d, 104(%rsp)
        movl    104(%rsp), %r10d
        movl    %r10d, 100(%rsp)
        movl    12(%rsp), %r10d
        movl    20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 108(%rsp)
        movl    108(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 112(%rsp)
        movl    112(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 116(%rsp)
        movl    116(%rsp), %r10d
        movl    36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 120(%rsp)
        movl    120(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 124(%rsp)
        movl    124(%rsp), %r10d
        movl    36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 128(%rsp)
        movl    128(%rsp), %r10d
        movl    44(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 132(%rsp)
        movl    132(%rsp), %r10d
        movl    52(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 136(%rsp)
        movl    136(%rsp), %r10d
        movl    60(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 140(%rsp)
        movl    140(%rsp), %r10d
        movl    92(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 144(%rsp)
        movl    144(%rsp), %r10d
        movl    100(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 148(%rsp)
        movl    148(%rsp), %r10d
        movl    8(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 152(%rsp)
        movl    152(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lcaller_saved_121:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $160, %rsp
        ret
.Lcaller_saved_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"