    .text
    .globl  fun_call
    .type   fun_call, @function
fun_call:
        subq    $8, %rsp
.Lfun_call_entry:
        movl    $42, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lfun_call_3:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $8, %rsp
        ret
.Lfun_call_end:
    .globl  mem2reg_basic
    .type   mem2reg_basic, @function
mem2reg_basic:
        subq    $28, %rsp
.Lmem2reg_basic_entry:
        movl    $0, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $5, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 24(%rsp)
.Lmem2reg_basic_13:
        movl    24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $28, %rsp
        ret
.Lmem2reg_basic_end:
    .globl  mem2reg_branching
    .type   mem2reg_branching, @function
mem2reg_branching:
        subq    $36, %rsp
        movl    %edi, %r10d
.Lmem2reg_branching_entry:
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    $42, 12(%rsp)
        movl    8(%rsp), %r10d
        movl    12(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lmem2reg_branching_18
.Lmem2reg_branching_15:
.Lmem2reg_branching_16:
        movl    $1, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 4(%rsp)
.Lmem2reg_branching_25:
        movl    4(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 28(%rsp)
.Lmem2reg_branching_28:
        movl    28(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lmem2reg_branching_18:
.Lmem2reg_branching_19:
        movl    32(%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    $2, 24(%rsp)
        movl    20(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        addq    $36, %rsp
        jmp     .Lmem2reg_branching_25
.Lmem2reg_branching_end:
    .globl  mem2reg_while
    .type   mem2reg_while, @function
mem2reg_while:
        subq    $48, %rsp
.Lmem2reg_while_entry:
        movl    $0, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $0, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lmem2reg_while_14:
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    $10, 20(%rsp)
        movl    16(%rsp), %r10d
        movl    20(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, 16(%rsp)
        movl    16(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lmem2reg_while_33
.Lmem2reg_while_20:
.Lmem2reg_while_21:
        movl    8(%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r11d
        movl    24(%rsp), %edi
        leal    (%r11d,%edi), %r10d
        movl    %r10d, 24(%rsp)
        movl    24(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 28(%rsp)
        movl    $1, 32(%rsp)
        movl    28(%rsp), %r10d
        movl    32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, (%rsp)
        jmp     .Lmem2reg_while_14
.Lmem2reg_while_33:
.Lmem2reg_while_34:
        movl    8(%rsp), %r10d
        movl    %r10d, 40(%rsp)
        movl    40(%rsp), %r10d
        movl    %r10d, 44(%rsp)
.Lmem2reg_while_37:
        movl    44(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
.Lmem2reg_while_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"