    .text
    .globl  simplify_cfg_delete_orphaned
    .type   simplify_cfg_delete_orphaned, @function
simplify_cfg_delete_orphaned:
.Lsimplify_cfg_delete_orphaned_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_delete_orphaned_2
.Lsimplify_cfg_delete_orphaned_0:
.Lsimplify_cfg_delete_orphaned_1:
        movl    $2, %r10d
.Lsimplify_cfg_delete_orphaned_4:
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_delete_orphaned_2:
.Lsimplify_cfg_delete_orphaned_3:
        movl    $3, %r10d
        jmp     .Lsimplify_cfg_delete_orphaned_4
.Lsimplify_cfg_delete_orphaned_end:
    .globl  simplify_cfg_merge_straight_line
    .type   simplify_cfg_merge_straight_line, @function
simplify_cfg_merge_straight_line:
.Lsimplify_cfg_merge_straight_line_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_merge_straight_line_4
.Lsimplify_cfg_merge_straight_line_0:
.Lsimplify_cfg_merge_straight_line_1:
        movq    $2, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_merge_straight_line_6
.Lsimplify_cfg_merge_straight_line_2:
.Lsimplify_cfg_merge_straight_line_3:
        movl    $3, %r10d
.Lsimplify_cfg_merge_straight_line_8:
        movl    %r10d, %eax
        ret
.Lsimplify_cfg_merge_straight_line_4:
.Lsimplify_cfg_merge_straight_line_5:
        movl    $5, %r10d
        jmp     .Lsimplify_cfg_merge_straight_line_8
.Lsimplify_cfg_merge_straight_line_6:
.Lsimplify_cfg_merge_straight_line_7:
        movl    $4, %r10d
        jmp     .Lsimplify_cfg_merge_straight_line_8
.Lsimplify_cfg_merge_straight_line_end:
    .globl  simplify_cfg_delete_empty
    .type   simplify_cfg_delete_empty, @function
simplify_cfg_delete_empty:
.Lsimplify_cfg_delete_empty_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_delete_empty_2
.Lsimplify_cfg_delete_empty_0:
.Lsimplify_cfg_delete_empty_1:
        movq    $2, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_delete_empty_4
.Lsimplify_cfg_delete_empty_4:
        movl    $4, %eax
        ret
.Lsimplify_cfg_delete_empty_2:
.Lsimplify_cfg_delete_empty_3:
        movq    $3, %r10
        cmpq    $0, %r10
        je      .Lsimplify_cfg_delete_empty_4
        jmp     .Lsimplify_cfg_delete_empty_4
.Lsimplify_cfg_delete_empty_end:
    .globl  simplify_cfg_empty_loop
    .type   simplify_cfg_empty_loop, @function
simplify_cfg_empty_loop:
.Lsimplify_cfg_empty_loop_entry:
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lsimplify_cfg_empty_loop_1
.Lsimplify_cfg_empty_loop_0:
        jmp     .Lsimplify_cfg_empty_loop_entry
.Lsimplify_cfg_empty_loop_1:
.Lsimplify_cfg_empty_loop_2:
        movl    $0, %eax
        ret
.Lsimplify_cfg_empty_loop_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"