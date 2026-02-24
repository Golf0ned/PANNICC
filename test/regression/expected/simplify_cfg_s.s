    .text
    .globl  simplify_cfg_delete_orphaned
    .type   simplify_cfg_delete_orphaned, @function
simplify_cfg_delete_orphaned:
        subq    $16, %rsp
.Lsimplify_cfg_delete_orphaned_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lsimplify_cfg_delete_orphaned_8
.Lsimplify_cfg_delete_orphaned_5:
.Lsimplify_cfg_delete_orphaned_6:
        movl    $2, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 12(%rsp)
.Lsimplify_cfg_delete_orphaned_11:
        movl    12(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_delete_orphaned_8:
.Lsimplify_cfg_delete_orphaned_9:
        movl    $3, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 12(%rsp)
        addq    $16, %rsp
        jmp     .Lsimplify_cfg_delete_orphaned_11
.Lsimplify_cfg_delete_orphaned_end:
    .globl  simplify_cfg_merge_straight_line
    .type   simplify_cfg_merge_straight_line, @function
simplify_cfg_merge_straight_line:
        subq    $40, %rsp
.Lsimplify_cfg_merge_straight_line_entry:
        movl    $0, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lsimplify_cfg_merge_straight_line_27
.Lsimplify_cfg_merge_straight_line_12:
.Lsimplify_cfg_merge_straight_line_13:
        movl    $2, 12(%rsp)
        movl    12(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lsimplify_cfg_merge_straight_line_18
.Lsimplify_cfg_merge_straight_line_15:
.Lsimplify_cfg_merge_straight_line_16:
        movl    $3, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, 36(%rsp)
.Lsimplify_cfg_merge_straight_line_30:
        movl    36(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_merge_straight_line_27:
.Lsimplify_cfg_merge_straight_line_28:
        movl    $5, 28(%rsp)
        movl    28(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        jmp     .Lsimplify_cfg_merge_straight_line_30
.Lsimplify_cfg_merge_straight_line_18:
.Lsimplify_cfg_merge_straight_line_19:
        movl    (%rsp), %r10d
        movl    %r10d, 20(%rsp)
        movl    $4, 24(%rsp)
        movl    20(%rsp), %r10d
        movl    24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, 20(%rsp)
        movl    20(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, 32(%rsp)
        movl    32(%rsp), %r10d
        movl    %r10d, 36(%rsp)
        addq    $40, %rsp
        jmp     .Lsimplify_cfg_merge_straight_line_30
.Lsimplify_cfg_merge_straight_line_end:
    .globl  simplify_cfg_delete_empty
    .type   simplify_cfg_delete_empty, @function
simplify_cfg_delete_empty:
        subq    $20, %rsp
.Lsimplify_cfg_delete_empty_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lsimplify_cfg_delete_empty_9
.Lsimplify_cfg_delete_empty_6:
.Lsimplify_cfg_delete_empty_7:
        movl    $2, 4(%rsp)
        movl    4(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lsimplify_cfg_delete_empty_12
.Lsimplify_cfg_delete_empty_12:
        movl    $4, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 16(%rsp)
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_delete_empty_9:
.Lsimplify_cfg_delete_empty_10:
        movl    $3, 8(%rsp)
        movl    8(%rsp), %r10d
        cmpl    $0, %r10d
        je      .Lsimplify_cfg_delete_empty_12
        addq    $20, %rsp
        jmp     .Lsimplify_cfg_delete_empty_12
.Lsimplify_cfg_delete_empty_end:
    .globl  simplify_cfg_empty_loop
    .type   simplify_cfg_empty_loop, @function
simplify_cfg_empty_loop:
        subq    $12, %rsp
.Lsimplify_cfg_empty_loop_entry:
.Lsimplify_cfg_empty_loop_4:
        movl    $0, (%rsp)
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lsimplify_cfg_empty_loop_6
.Lsimplify_cfg_empty_loop_3:
        jmp     .Lsimplify_cfg_empty_loop_4
.Lsimplify_cfg_empty_loop_6:
.Lsimplify_cfg_empty_loop_7:
        movl    $0, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lsimplify_cfg_empty_loop_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"