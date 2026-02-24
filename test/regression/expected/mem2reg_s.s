    .text
    .globl  fun_call
    .type   fun_call, @function
fun_call:
.Lfun_call_entry:
.Lfun_call_0:
        movl    $42, %eax
        ret
.Lfun_call_end:
    .globl  mem2reg_basic
    .type   mem2reg_basic, @function
mem2reg_basic:
.Lmem2reg_basic_entry:
.Lmem2reg_basic_0:
        movl    $5, %eax
        ret
.Lmem2reg_basic_end:
    .globl  mem2reg_branching
    .type   mem2reg_branching, @function
mem2reg_branching:
        movl    %edi, %r11d
.Lmem2reg_branching_entry:
        movl    %r11d, %r10d
        subl    $42, %r10d
        cmpl    $0, %r10d
        jne     .Lmem2reg_branching_4
.Lmem2reg_branching_2:
.Lmem2reg_branching_3:
        movl    $1, %r11d
.Lmem2reg_branching_7:
.Lmem2reg_branching_9:
        movl    %r11d, %eax
        ret
.Lmem2reg_branching_4:
.Lmem2reg_branching_5:
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, %r11d
        jmp     .Lmem2reg_branching_7
.Lmem2reg_branching_end:
    .globl  mem2reg_while
    .type   mem2reg_while, @function
mem2reg_while:
.Lmem2reg_while_entry:
        movl    $0, %edi
        movl    $0, %r11d
.Lmem2reg_while_0:
        movl    %r11d, %r10d
        subl    $10, %r10d
        cmpl    $0, %r10d
        jne     .Lmem2reg_while_8
.Lmem2reg_while_4:
.Lmem2reg_while_5:
        leal    (%edi,%r11d), %esi
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %esi, %edi
        movl    %r10d, %r11d
        jmp     .Lmem2reg_while_0
.Lmem2reg_while_8:
.Lmem2reg_while_9:
.Lmem2reg_while_10:
        movl    %edi, %eax
        ret
.Lmem2reg_while_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"