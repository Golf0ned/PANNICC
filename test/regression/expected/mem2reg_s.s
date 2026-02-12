fun_call: # 0 params, 0 stack bytes
.fun_call_entry:
.fun_call_0:
        movl    $42, %eax
        ret

mem2reg_basic: # 0 params, 0 stack bytes
.mem2reg_basic_entry:
.mem2reg_basic_0:
        movl    $5, %eax
        ret

mem2reg_branching: # 1 params, 0 stack bytes
        movl    %edi, %r11d
.mem2reg_branching_entry:
        movl    %r11d, %r10d
        subl    $42, %r10d
        cmpl    %r10d, $0
        jne     .mem2reg_branching_4
.mem2reg_branching_2:
.mem2reg_branching_3:
        movl    $1, %r11d
.mem2reg_branching_7:
.mem2reg_branching_9:
        movl    %r11d, %eax
        ret
.mem2reg_branching_4:
.mem2reg_branching_5:
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, %r11d
        jmp     .mem2reg_branching_7

mem2reg_while: # 0 params, 0 stack bytes
.mem2reg_while_entry:
        movl    $0, %edi
        movl    $0, %r11d
.mem2reg_while_0:
        movl    %r11d, %r10d
        subl    $10, %r10d
        cmpl    %r10d, $0
        jne     .mem2reg_while_8
.mem2reg_while_4:
.mem2reg_while_5:
        leal    (%edi,%r11d), %esi
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %esi, %edi
        movl    %r10d, %r11d
        jmp     .mem2reg_while_0
.mem2reg_while_8:
.mem2reg_while_9:
.mem2reg_while_10:
        movl    %edi, %eax
        ret