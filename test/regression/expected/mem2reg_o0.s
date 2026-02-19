fun_call:
        subq    $16, %rsp
.fun_call_entry:
        movl    $42, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $4(%rsp)
.fun_call_3:
        movl    $4(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret
mem2reg_basic:
        subq    $32, %rsp
.mem2reg_basic_entry:
        movl    $0, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $5, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $24(%rsp)
.mem2reg_basic_13:
        movl    $24(%rsp), %r10d
        movl    %r10d, %eax
        addq    $32, %rsp
        ret
mem2reg_branching:
        subq    $48, %rsp
        movl    %edi, %r10d
.mem2reg_branching_entry:
        movl    %r10d, $32(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $42, $12(%rsp)
        movl    $8(%rsp), %r10d
        movl    $12(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $8(%rsp)
        movl    $8(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .mem2reg_branching_18
.mem2reg_branching_15:
.mem2reg_branching_16:
        movl    $1, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $4(%rsp)
.mem2reg_branching_25:
        movl    $4(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $28(%rsp)
.mem2reg_branching_28:
        movl    $28(%rsp), %r10d
        movl    %r10d, %eax
        ret
.mem2reg_branching_18:
.mem2reg_branching_19:
        movl    $32(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $2, $24(%rsp)
        movl    $20(%rsp), %r10d
        movl    $24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        addq    $48, %rsp
        jmp     .mem2reg_branching_25
mem2reg_while:
        subq    $48, %rsp
.mem2reg_while_entry:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $0, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
.mem2reg_while_14:
        movl    (%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $10, $20(%rsp)
        movl    $16(%rsp), %r10d
        movl    $20(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $16(%rsp)
        movl    $16(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .mem2reg_while_33
.mem2reg_while_20:
.mem2reg_while_21:
        movl    $8(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $24(%rsp), %r11d
        movl    $24(%rsp), %edi
        leal    (%r11d,%edi), %r10d
        movl    %r10d, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $1, $32(%rsp)
        movl    $28(%rsp), %r10d
        movl    $32(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $28(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, (%rsp)
        jmp     .mem2reg_while_14
.mem2reg_while_33:
.mem2reg_while_34:
        movl    $8(%rsp), %r10d
        movl    %r10d, $40(%rsp)
        movl    $40(%rsp), %r10d
        movl    %r10d, $44(%rsp)
.mem2reg_while_37:
        movl    $44(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret