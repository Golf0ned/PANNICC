    .text
    .globl  while_basic
    .type   while_basic, @function
while_basic:
        subq    $12, %rsp
.Lwhile_basic_entry:
        movl    $0, (%rsp)
.Lwhile_basic_3:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lwhile_basic_9
.Lwhile_basic_4:
.Lwhile_basic_5:
        movl    (%rsp), %r10d
        movl    %r10d, %r11d
        addl    $2, %r11d
        movl    %r11d, 4(%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, (%rsp)
        jmp     .Lwhile_basic_3
.Lwhile_basic_9:
.Lwhile_basic_10:
        movl    (%rsp), %r10d
        movl    %r10d, 8(%rsp)
.Lwhile_basic_12:
        movl    8(%rsp), %r10d
        movl    %r10d, %eax
        addq    $12, %rsp
        ret
.Lwhile_basic_end:
    .globl  while_complex
    .type   while_complex, @function
while_complex:
        subq    $20, %rsp
.Lwhile_complex_entry:
        movl    $0, (%rsp)
        movl    $100, 4(%rsp)
.Lwhile_complex_5:
        movl    4(%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lwhile_complex_15
.Lwhile_complex_7:
.Lwhile_complex_8:
        movl    (%rsp), %r10d
        movl    %r10d, %r11d
        addl    $5, %r11d
        movl    %r11d, 8(%rsp)
        movl    8(%rsp), %r10d
        movl    %r10d, (%rsp)
        movl    4(%rsp), %r10d
        movl    %r10d, %r11d
        subl    $1, %r11d
        movl    %r11d, 12(%rsp)
        movl    12(%rsp), %r10d
        movl    %r10d, 4(%rsp)
        jmp     .Lwhile_complex_5
.Lwhile_complex_15:
.Lwhile_complex_16:
        movl    (%rsp), %r10d
        movl    %r10d, 16(%rsp)
.Lwhile_complex_18:
        movl    16(%rsp), %r10d
        movl    %r10d, %eax
        addq    $20, %rsp
        ret
.Lwhile_complex_end:
    .globl  while_empty
    .type   while_empty, @function
while_empty:
        subq    $4, %rsp
.Lwhile_empty_entry:
.Lwhile_empty_1:
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lwhile_empty_4
.Lwhile_empty_2:
.Lwhile_empty_3:
        jmp     .Lwhile_empty_1
.Lwhile_empty_4:
.Lwhile_empty_5:
        movl    $0, (%rsp)
.Lwhile_empty_6:
        movl    (%rsp), %r10d
        movl    %r10d, %eax
        addq    $4, %rsp
        ret
.Lwhile_empty_end:
    .globl  while_if
    .type   while_if, @function
while_if:
        subq    $8, %rsp
.Lwhile_if_entry:
        movl    $200, (%rsp)
.Lwhile_if_2:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lwhile_if_12
.Lwhile_if_3:
.Lwhile_if_4:
        movl    (%rsp), %r10d
        cmpl    $0, %r10d
        jne     .Lwhile_if_8
.Lwhile_if_6:
.Lwhile_if_7:
.Lwhile_if_11:
        jmp     .Lwhile_if_2
.Lwhile_if_12:
.Lwhile_if_13:
        movl    $0, 4(%rsp)
.Lwhile_if_14:
        movl    4(%rsp), %r10d
        movl    %r10d, %eax
        ret
.Lwhile_if_8:
.Lwhile_if_9:
        movl    $500, 4(%rsp)
        addq    $8, %rsp
        jmp     .Lwhile_if_14
.Lwhile_if_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"