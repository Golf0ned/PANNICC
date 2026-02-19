lost_copy_problem:
        subq    $48, %rsp
        movl    %edi, %r10d
.lost_copy_problem_entry:
        movl    %r10d, $32(%rsp)
.lost_copy_problem_10:
        movl    $32(%rsp), %r10d
        movl    %r10d, $4(%rsp)
        movl    $10, $8(%rsp)
        movl    $4(%rsp), %r10d
        movl    $8(%rsp), %edi
        movl    %r10d, %r11d
        subl    %edi, %r11d
        movl    %r11d, $4(%rsp)
        movl    $4(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .lost_copy_problem_23
.lost_copy_problem_16:
.lost_copy_problem_17:
        movl    $32(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    $1, $16(%rsp)
        movl    $32(%rsp), %r10d
        movl    $16(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $32(%rsp)
        jmp     .lost_copy_problem_10
.lost_copy_problem_23:
.lost_copy_problem_24:
        movl    (%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $3, $24(%rsp)
        movl    $20(%rsp), %r10d
        movl    $24(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $28(%rsp)
.lost_copy_problem_30:
        movl    $28(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret
swap_problem:
        subq    $48, %rsp
        movl    %edi, %r11d
        movl    %esi, %r10d
.swap_problem_entry:
        movl    %r11d, $32(%rsp)
        movl    %r10d, $36(%rsp)
.swap_problem_12:
        movl    $1, $4(%rsp)
        movl    $4(%rsp), %r10d
        cmpl    %r10d, $0
        jne     .swap_problem_22
.swap_problem_14:
.swap_problem_15:
        movl    $32(%rsp), %r10d
        movl    %r10d, $12(%rsp)
        movl    $12(%rsp), %r10d
        movl    %r10d, $8(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $16(%rsp)
        movl    $16(%rsp), %r10d
        movl    %r10d, $32(%rsp)
        movl    $8(%rsp), %r10d
        movl    %r10d, $20(%rsp)
        movl    $20(%rsp), %r10d
        movl    %r10d, $36(%rsp)
        jmp     .swap_problem_12
.swap_problem_22:
.swap_problem_23:
        movl    $32(%rsp), %r10d
        movl    %r10d, $24(%rsp)
        movl    $36(%rsp), %r10d
        movl    %r10d, $28(%rsp)
        movl    $24(%rsp), %r10d
        movl    $28(%rsp), %r11d
        leal    (%r10d,%r11d), %edi
        movl    %edi, $24(%rsp)
        movl    $24(%rsp), %r10d
        movl    %r10d, $32(%rsp)
.swap_problem_30:
        movl    $32(%rsp), %r10d
        movl    %r10d, %eax
        addq    $48, %rsp
        ret