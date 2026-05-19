    .text
    .globl  simplify_cfg_delete_orphaned
    .type   simplify_cfg_delete_orphaned, @function
simplify_cfg_delete_orphaned:
        subq    $4, %rsp
.Lsimplify_cfg_delete_orphaned_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_delete_orphaned_3
.Lsimplify_cfg_delete_orphaned_1:
.Lsimplify_cfg_delete_orphaned_2:
        movl    $2, (%rsp)
.Lsimplify_cfg_delete_orphaned_5:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_delete_orphaned_3:
.Lsimplify_cfg_delete_orphaned_4:
        movl    $3, (%rsp)
        addq    $4, %rsp
        jmp     .Lsimplify_cfg_delete_orphaned_5
.Lsimplify_cfg_delete_orphaned_end:
    .globl  simplify_cfg_merge_straight_line
    .type   simplify_cfg_merge_straight_line, @function
simplify_cfg_merge_straight_line:
        subq    $12, %rsp
.Lsimplify_cfg_merge_straight_line_entry:
        movl    $0, 4(%rsp)
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_merge_straight_line_13
.Lsimplify_cfg_merge_straight_line_3:
.Lsimplify_cfg_merge_straight_line_4:
        movq    $2, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_merge_straight_line_7
.Lsimplify_cfg_merge_straight_line_5:
.Lsimplify_cfg_merge_straight_line_6:
        movl    $3, (%rsp)
.Lsimplify_cfg_merge_straight_line_15:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_merge_straight_line_13:
.Lsimplify_cfg_merge_straight_line_14:
        movl    $5, (%rsp)
        jmp     .Lsimplify_cfg_merge_straight_line_15
.Lsimplify_cfg_merge_straight_line_7:
.Lsimplify_cfg_merge_straight_line_8:
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        addl    $4, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        addq    $12, %rsp
        jmp     .Lsimplify_cfg_merge_straight_line_15
.Lsimplify_cfg_merge_straight_line_end:
    .globl  simplify_cfg_delete_empty
    .type   simplify_cfg_delete_empty, @function
simplify_cfg_delete_empty:
        subq    $4, %rsp
.Lsimplify_cfg_delete_empty_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_delete_empty_3
.Lsimplify_cfg_delete_empty_1:
.Lsimplify_cfg_delete_empty_2:
        movq    $2, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_delete_empty_5
.Lsimplify_cfg_delete_empty_5:
        movl    $4, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_delete_empty_3:
.Lsimplify_cfg_delete_empty_4:
        movq    $3, %r10
        cmpq    $0, %r10
        je      .Lsimplify_cfg_delete_empty_5
        addq    $4, %rsp
        jmp     .Lsimplify_cfg_delete_empty_5
.Lsimplify_cfg_delete_empty_end:
    .globl  simplify_cfg_empty_loop
    .type   simplify_cfg_empty_loop, @function
simplify_cfg_empty_loop:
        subq    $4, %rsp
.Lsimplify_cfg_empty_loop_entry:
.Lsimplify_cfg_empty_loop_2:
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_empty_loop_3
.Lsimplify_cfg_empty_loop_1:
        jmp     .Lsimplify_cfg_empty_loop_2
.Lsimplify_cfg_empty_loop_3:
.Lsimplify_cfg_empty_loop_4:
        movl    $0, (%rsp)
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Lsimplify_cfg_empty_loop_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"