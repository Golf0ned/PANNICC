    .text
    .globl  while_basic
    .type   while_basic, @function
while_basic:
.Lwhile_basic_entry:
        movl    $0, %r10d
.Lwhile_basic_0:
        movq    $1, %r11
        cmpq    $0, %r11
        jne     .Lwhile_basic_5
.Lwhile_basic_2:
.Lwhile_basic_3:
        movl    %r10d, %r11d
        addl    $2, %r11d
        movl    %r11d, %r10d
        jmp     .Lwhile_basic_0
.Lwhile_basic_5:
.Lwhile_basic_6:
        movl    %r10d, %eax
        ret
.Lwhile_basic_end:
    .globl  while_complex
    .type   while_complex, @function
while_complex:
.Lwhile_complex_entry:
        movl    $0, %r11d
        movl    $100, %r10d
.Lwhile_complex_0:
        cmpl    $0, %r10d
        jne     .Lwhile_complex_7
.Lwhile_complex_3:
.Lwhile_complex_4:
        movl    %r11d, %edi
        addl    $5, %edi
        movl    %r10d, %esi
        subl    $1, %esi
        movl    %edi, %r11d
        movl    %esi, %r10d
        jmp     .Lwhile_complex_0
.Lwhile_complex_7:
.Lwhile_complex_8:
        movl    %r11d, %eax
        ret
.Lwhile_complex_end:
    .globl  while_empty
    .type   while_empty, @function
while_empty:
.Lwhile_empty_entry:
        movq    $0, %r10
        cmpq    $0, %r10
        jne     .Lwhile_empty_1
.Lwhile_empty_0:
        jmp     .Lwhile_empty_entry
.Lwhile_empty_1:
.Lwhile_empty_2:
        movl    $0, %eax
        ret
.Lwhile_empty_end:
    .globl  while_if
    .type   while_if, @function
while_if:
.Lwhile_if_entry:
        movq    $1, %r10
        cmpq    $0, %r10
        jne     .Lwhile_if_5
.Lwhile_if_0:
.Lwhile_if_1:
        movq    $200, %r10
        cmpq    $0, %r10
        je      .Lwhile_if_4
.Lwhile_if_2:
.Lwhile_if_3:
        movl    $500, %r10d
.Lwhile_if_7:
        movl    %r10d, %eax
        ret
.Lwhile_if_5:
.Lwhile_if_6:
        movl    $0, %r10d
        jmp     .Lwhile_if_7
.Lwhile_if_4:
        jmp     .Lwhile_if_entry
.Lwhile_if_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"