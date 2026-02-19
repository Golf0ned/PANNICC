simplify_cfg_delete_orphaned:
        subq    $16, %rsp
.simplify_cfg_delete_orphaned_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_delete_orphaned_8
.simplify_cfg_delete_orphaned_5:
.simplify_cfg_delete_orphaned_6:
        movl    $2, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $12(%rsp)
.simplify_cfg_delete_orphaned_12:
        movl    $12(%rsp), %r10d
        movl    %r10d, %eax
        ret
.simplify_cfg_delete_orphaned_8:
.simplify_cfg_delete_orphaned_9:
        movl    $3, $8(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        addq    $16, %rsp
        jmp     .simplify_cfg_delete_orphaned_12
simplify_cfg_merge_straight_line:
        subq    $48, %rsp
.simplify_cfg_merge_straight_line_entry:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, $8(%rsp)
        movl    $8(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_merge_straight_line_25
.simplify_cfg_merge_straight_line_12:
.simplify_cfg_merge_straight_line_13:
        movl    $2, $12(%rsp)
        movl    $12(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_merge_straight_line_18
.simplify_cfg_merge_straight_line_15:
.simplify_cfg_merge_straight_line_16:
        movl    $3, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $36(%rsp)
.simplify_cfg_merge_straight_line_31:
        movl    $36(%rsp), %r10d
        movl    %r10d, %eax
        ret
.simplify_cfg_merge_straight_line_25:
.simplify_cfg_merge_straight_line_26:
        movl    $5, $28(%rsp)
        movl    $28(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        jmp     .simplify_cfg_merge_straight_line_31
.simplify_cfg_merge_straight_line_18:
.simplify_cfg_merge_straight_line_19:
        movl    (%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $4, $24(%rsp)
        movl    $20(%rsp), %r10d
        movl    $24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, (%rsp)
.simplify_cfg_merge_straight_line_28:
        movl    (%rsp), %r10d
        movl    %r10d, $32(%rsp)
        movl    $32(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        addq    $48, %rsp
        jmp     .simplify_cfg_merge_straight_line_31
simplify_cfg_delete_empty:
        subq    $32, %rsp
.simplify_cfg_delete_empty_entry:
        movl    $1, (%rsp)
        movl    (%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_delete_empty_14
.simplify_cfg_delete_empty_6:
.simplify_cfg_delete_empty_7:
        movl    $2, $4(%rsp)
        movl    $4(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_delete_empty_11
.simplify_cfg_delete_empty_9:
.simplify_cfg_delete_empty_10:
.simplify_cfg_delete_empty_13:
.simplify_cfg_delete_empty_22:
        movl    $4, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $16(%rsp)
.simplify_cfg_delete_empty_24:
        movl    $16(%rsp), %r10d
        movl    %r10d, %eax
        ret
.simplify_cfg_delete_empty_14:
.simplify_cfg_delete_empty_15:
        movl    $3, $8(%rsp)
        movl    $8(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_delete_empty_19
.simplify_cfg_delete_empty_17:
.simplify_cfg_delete_empty_18:
.simplify_cfg_delete_empty_21:
        jmp     .simplify_cfg_delete_empty_22
.simplify_cfg_delete_empty_11:
.simplify_cfg_delete_empty_12:
        jmp     .simplify_cfg_delete_empty_13
.simplify_cfg_delete_empty_19:
.simplify_cfg_delete_empty_20:
        addq    $32, %rsp
        jmp     .simplify_cfg_delete_empty_21
simplify_cfg_empty_loop:
        subq    $16, %rsp
.simplify_cfg_empty_loop_entry:
.simplify_cfg_empty_loop_3:
        movl    $0, (%rsp)
        movl    (%rsp), %r10d
        cmpl    %r10d, $0
        jne     .simplify_cfg_empty_loop_7
.simplify_cfg_empty_loop_5:
.simplify_cfg_empty_loop_6:
        jmp     .simplify_cfg_empty_loop_3
.simplify_cfg_empty_loop_7:
.simplify_cfg_empty_loop_8:
        movl    $0, $4(%rsp)
        movl    $4(%rsp), %r10d
        movl    %r10d, $8(%rsp)
.simplify_cfg_empty_loop_10:
        movl    $8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $16, %rsp
        ret