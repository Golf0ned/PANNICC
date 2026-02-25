    .text
    .globl  fun1
    .type   fun1, @function
fun1:
        subq    $16, %rsp
        movl    %edi, %r10d
.Lfun1_entry:
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    (%rsp), %r10d
        movl    4(%rsp), %edi
        movl    %r10d, %r11d
        imull   %edi, %r11d
        movl    %r11d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lfun1_11:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lfun1_end:
    .globl  fun2
    .type   fun2, @function
fun2:
        subq    $48, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lfun2_entry:
        movl    %r11d, 32(%rsp)
        movl    %r10d, 36(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 8(%rsp)
        movl    $2, 12(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r11d
        movl    12(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 12(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    12(%rsp), %r11d
        movl    16(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 12(%rsp)
        movl    8(%rsp), %r10d
        movl    12(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 8(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 24(%rsp)
        movl    8(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 28(%rsp)
.Lfun2_33:
        movl    28(%rsp), %r10d
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
        movl    %esi, 36(%rsp)
        movl    %r11d, 32(%rsp)
        movl    %edi, 36(%rsp)
        movl    %r10d, 40(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 16(%rsp)
        movl    12(%rsp), %r11d
        movl    16(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 12(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    20(%rsp), %r11d
        movl    24(%rsp), %edi
        movl    %r11d, %r10d
        imull   %edi, %r10d
        movl    %r10d, 20(%rsp)
        movl    12(%rsp), %r10d
        movl    20(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 12(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    12(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 32(%rsp)
.Lfun3_35:
        movl    32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
.Lfun3_end:
    .globl  call1
    .type   call1, @function
call1:
        subq    $16, %rsp
.Lcall1_entry:
        movl    $5, (%rsp)
        movl    (%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lcall1_6:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall1_end:
    .globl  call2
    .type   call2, @function
call2:
        subq    $32, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall2_entry:
        movl    %r11d, 16(%rsp)
        movl    %r10d, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    4(%rsp), %r10d
        movl    8(%rsp), %r11d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r10d, %edi
        movl    %r11d, %esi
        call    fun2
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lcall2_14:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Lcall2_end:
    .globl  call3
    .type   call3, @function
call3:
        subq    $32, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall3_entry:
        movl    %r11d, 24(%rsp)
        movl    %r10d, 28(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    $1, 12(%rsp)
        movl    $0, 12(%rsp)
        movl    4(%rsp), %r11d
        movl    8(%rsp), %r10d
        movl    12(%rsp), %edi
        movl    12(%rsp), %esi
        subq    $32, %rsp
        movq    %rdi, 24(%rsp)
        movq    %rsi, 16(%rsp)
        movq    %r10, 8(%rsp)
        movq    %r11, (%rsp)
        movl    %r11d, %edi
        movl    %r10d, %esi
        movl    %edi, %edx
        movl    %esi, %ecx
        call    fun3
        addq    $32, %rsp
        movl    %eax, %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 20(%rsp)
.Lcall3_18:
        movl    20(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
.Lcall3_end:
    .globl  call4
    .type   call4, @function
call4:
        subq    $16, %rsp
.Lcall4_entry:
        movl    $10, (%rsp)
        movl    (%rsp), %r10d
        subq    $16, %rsp
        movq    %r10, 8(%rsp)
        movl    %r10d, %edi
        call    fun1
        addq    $16, %rsp
        movl    %eax, %r10d
        movl    %r10d, (%rsp)
        movl    $100, 4(%rsp)
        movl    (%rsp), %r11d
        movl    4(%rsp), %edi
        movl    %r11d, %r10d
        subl    %edi, %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lcall4_10:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
.Lcall4_end:
    .globl  super_fun
    .type   super_fun, @function
super_fun:
        subq    $68, %rsp
        movl    %edi, %r9d
        movl    %esi, %edi
        movl    %edx, %r10d
        movl    %ecx, %edx
        movl    %r8d, %r8d
        movl    %r9d, %ecx
        movl    68(%rsp), %esi
        movl    76(%rsp), %r11d
.Lsuper_fun_entry:
        movl    %r9d, 36(%rsp)
        movl    %edi, 40(%rsp)
        movl    %r10d, 44(%rsp)
        movl    %edx, 48(%rsp)
        movl    %r8d, 52(%rsp)
        movl    %ecx, 56(%rsp)
        movl    %esi, 60(%rsp)
        movl    %r11d, 64(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    24(%rsp), %edi
        movl    28(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 24(%rsp)
        movl    44(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    24(%rsp), %r11d
        movl    32(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 24(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    24(%rsp), %r10d
        movl    36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 24(%rsp)
        movl    52(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    24(%rsp), %r10d
        movl    32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 24(%rsp)
        movl    56(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    24(%rsp), %r10d
        movl    36(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 24(%rsp)
        movl    60(%rsp), %r10d
        movl    %r10d, 40(%rsp)
        movl    24(%rsp), %r10d
        movl    40(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 24(%rsp)
        movl    64(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    24(%rsp), %r10d
        movl    28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 32(%rsp)
.Lsuper_fun_55:
        movl    32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $68, %rsp
        ret
.Lsuper_fun_end:
    .globl  call_super_fun
    .type   call_super_fun, @function
call_super_fun:
        subq    $48, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcall_super_fun_entry:
        movl    %r11d, 32(%rsp)
        movl    %r10d, 36(%rsp)
        movl    $1, 4(%rsp)
        movl    $2, 8(%rsp)
        movl    $3, 12(%rsp)
        movl    $4, 12(%rsp)
        movl    $5, 16(%rsp)
        movl    $6, 20(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    4(%rsp), %esi
        movl    8(%rsp), %r11d
        movl    12(%rsp), %r10d
        movl    12(%rsp), %edi
        movl    16(%rsp), %edx
        movl    20(%rsp), %ecx
        movl    24(%rsp), %r8d
        movl    28(%rsp), %r9d
        subq    $80, %rsp
        movq    %rdi, 72(%rsp)
        movq    %rsi, 64(%rsp)
        movq    %rdx, 56(%rsp)
        movq    %rcx, 48(%rsp)
        movq    %r8, 40(%rsp)
        movq    %r9, 32(%rsp)
        movq    %r10, 24(%rsp)
        movq    %r11, 16(%rsp)
        movl    %esi, %edi
        movl    %r11d, %esi
        movl    %r10d, %edx
        movl    %edi, %ecx
        movl    %edx, %r8d
        movl    %ecx, %r9d
        movl    %r8d, (%rsp)
        movl    %r9d, 8(%rsp)
        call    super_fun
        addq    $80, %rsp
        movl    %eax, %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 36(%rsp)
.Lcall_super_fun_26:
        movl    36(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
.Lcall_super_fun_end:
    .globl  caller_saved
    .type   caller_saved, @function
caller_saved:
        subq    $240, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.Lcaller_saved_entry:
        movl    %r11d, 228(%rsp)
        movl    %r10d, 232(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    $1, 12(%rsp)
        movl    8(%rsp), %edi
        movl    12(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    $2, 20(%rsp)
        movl    16(%rsp), %edi
        movl    20(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    $3, 32(%rsp)
        movl    28(%rsp), %edi
        movl    32(%rsp), %r11d
        leal    (%edi,%r11d), %r10d
        movl    %r10d, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    $4, 36(%rsp)
        movl    32(%rsp), %r11d
        movl    36(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    $5, 32(%rsp)
        movl    28(%rsp), %r11d
        movl    32(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 40(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 40(%rsp)
        movl    $6, 44(%rsp)
        movl    40(%rsp), %r11d
        movl    44(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 52(%rsp)
        movl    $7, 56(%rsp)
        movl    52(%rsp), %r11d
        movl    56(%rsp), %r10d
        leal    (%r11d,%r10d), %edi
        movl    %edi, 52(%rsp)
        movl    52(%rsp), %r10d
        movl    %r10d, 48(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 64(%rsp)
        movl    $8, 92(%rsp)
        movl    64(%rsp), %r10d
        movl    92(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 64(%rsp)
        movl    64(%rsp), %r10d
        movl    %r10d, 60(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 100(%rsp)
        movl    $9, 104(%rsp)
        movl    100(%rsp), %r10d
        movl    104(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 100(%rsp)
        movl    100(%rsp), %r10d
        movl    %r10d, 96(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 112(%rsp)
        movl    $10, 116(%rsp)
        movl    112(%rsp), %r10d
        movl    116(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 112(%rsp)
        movl    112(%rsp), %r10d
        movl    %r10d, 108(%rsp)
        movl    228(%rsp), %r10d
        movl    %r10d, 124(%rsp)
        movl    $11, 128(%rsp)
        movl    124(%rsp), %r10d
        movl    128(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 124(%rsp)
        movl    124(%rsp), %r10d
        movl    %r10d, 120(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 136(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 140(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 144(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 148(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 152(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 156(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 160(%rsp)
        movl    60(%rsp), %r10d
        movl    %r10d, 164(%rsp)
        movl    136(%rsp), %r10d
        movl    140(%rsp), %r11d
        movl    144(%rsp), %edi
        movl    148(%rsp), %esi
        movl    152(%rsp), %edx
        movl    156(%rsp), %ecx
        movl    160(%rsp), %r8d
        movl    164(%rsp), %r9d
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
        movl    %r10d, 168(%rsp)
        movl    168(%rsp), %r10d
        movl    %r10d, 132(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 172(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 176(%rsp)
        movl    172(%rsp), %r10d
        movl    176(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 180(%rsp)
        movl    172(%rsp), %r10d
        movl    180(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 184(%rsp)
        movl    172(%rsp), %r10d
        movl    184(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 188(%rsp)
        movl    172(%rsp), %r10d
        movl    188(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    36(%rsp), %r10d
        movl    %r10d, 192(%rsp)
        movl    172(%rsp), %r10d
        movl    192(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    48(%rsp), %r10d
        movl    %r10d, 196(%rsp)
        movl    172(%rsp), %r10d
        movl    196(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    60(%rsp), %r10d
        movl    %r10d, 200(%rsp)
        movl    172(%rsp), %r10d
        movl    200(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    96(%rsp), %r10d
        movl    %r10d, 204(%rsp)
        movl    172(%rsp), %r10d
        movl    204(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    108(%rsp), %r10d
        movl    %r10d, 208(%rsp)
        movl    172(%rsp), %r10d
        movl    208(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    120(%rsp), %r10d
        movl    %r10d, 212(%rsp)
        movl    172(%rsp), %r10d
        movl    212(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    132(%rsp), %r10d
        movl    %r10d, 216(%rsp)
        movl    172(%rsp), %r10d
        movl    216(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    232(%rsp), %r10d
        movl    %r10d, 220(%rsp)
        movl    172(%rsp), %r10d
        movl    220(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 172(%rsp)
        movl    172(%rsp), %r10d
        movl    %r10d, 224(%rsp)
.Lcaller_saved_184:
        movl    224(%rsp), %r10d
        movl    %r10d, %eax
        addq    $240, %rsp
        ret
.Lcaller_saved_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"