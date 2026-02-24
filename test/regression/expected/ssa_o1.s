    .text
    .globl  lost_copy_problem
    .type   lost_copy_problem, @function
lost_copy_problem:
        movl    %edi, %r10d
.Llost_copy_problem_entry:
        movl    %r10d, %r11d
.Llost_copy_problem_1:
        movl    %r11d, %r10d
        subl    $10, %r10d
        cmpl    $0, %r10d
        jne     .Llost_copy_problem_7
.Llost_copy_problem_4:
.Llost_copy_problem_5:
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %r10d, %r11d
        jmp     .Llost_copy_problem_1
.Llost_copy_problem_7:
.Llost_copy_problem_8:
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, %eax
        ret
.Llost_copy_problem_end:
    .globl  swap_problem
    .type   swap_problem, @function
swap_problem:
        movl    %edi, %r10d
        movl    %esi, %edi
.Lswap_problem_entry:
        movl    %r10d, %r11d
        movl    %edi, %r10d
.Lswap_problem_2:
        movq    $1, %rdi
        cmpq    $0, %rdi
        jne     .Lswap_problem_7
.Lswap_problem_5:
.Lswap_problem_6:
        movl    %r10d, %edi
        movl    %r11d, %r10d
        movl    %edi, %r11d
        jmp     .Lswap_problem_2
.Lswap_problem_7:
.Lswap_problem_8:
        leal    (%r11d,%r10d), %edi
        movl    %edi, %eax
        ret
.Lswap_problem_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"