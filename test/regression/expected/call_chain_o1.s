first:
.first_entry:
        movl    $1, %eax
        ret
second:
.second_entry:
        call    first
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %r10d, %eax
        ret
third:
.third_entry:
        call    second
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, %eax
        ret
fourth:
.fourth_entry:
        call    third
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, %eax
        ret
fifth:
.fifth_entry:
        call    fourth
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $4, %r10d
        movl    %r10d, %eax
        ret
sixth:
.sixth_entry:
        call    fifth
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $5, %r10d
        movl    %r10d, %eax
        ret
main:
.main_entry:
        call    sixth
        movl    %eax, %r10d
        call    first
        movl    %eax, %r10d
        movl    $0, %r11d
        addl    %r10d, %r11d
        subq    $16, %rsp
        call    second
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        call    third
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        call    fourth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        call    fifth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        call    sixth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        call    first
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        call    second
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        call    third
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        call    fourth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        call    fifth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %esi
        subq    $16, %rsp
        call    sixth
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (%esi,%r11d), %r10d
        movl    %r10d, %eax
        ret