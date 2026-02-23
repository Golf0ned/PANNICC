first:
.Lfirst_entry:
        movl    $1, %eax
        ret
second:
.Lsecond_entry:
        call    first
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $1, %r10d
        movl    %r10d, %eax
        ret
third:
.Lthird_entry:
        call    second
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $2, %r10d
        movl    %r10d, %eax
        ret
fourth:
.Lfourth_entry:
        call    third
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $3, %r10d
        movl    %r10d, %eax
        ret
fifth:
.Lfifth_entry:
        call    fourth
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $4, %r10d
        movl    %r10d, %eax
        ret
sixth:
.Lsixth_entry:
        call    fifth
        movl    %eax, %r11d
        movl    %r11d, %r10d
        addl    $5, %r10d
        movl    %r10d, %eax
        ret
main:
.Lmain_entry:
        call    sixth
        movl    %eax, %r10d
        call    first
        movl    %eax, %r10d
        movl    $0, %r11d
        addl    %r10d, %r11d
        subq    $16, %rsp
        movq    %r11d, $8(%rsp)
        call    second
        movq    $8(%rsp), %r11d
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %edi, $8(%rsp)
        call    third
        movq    $8(%rsp), %edi
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        call    fourth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %edi, $8(%rsp)
        movq    %r10d, (%rsp)
        call    fifth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        call    sixth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %edi, $8(%rsp)
        movq    %r10d, (%rsp)
        call    first
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        call    second
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %edi, $8(%rsp)
        movq    %r10d, (%rsp)
        call    third
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %r11d
        subq    $16, %rsp
        movq    %r10d, $8(%rsp)
        movq    %r11d, (%rsp)
        call    fourth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%r11d,%r10d), %edi
        subq    $16, %rsp
        movq    %edi, $8(%rsp)
        movq    %r10d, (%rsp)
        call    fifth
        addq    $16, %rsp
        movl    %eax, %r10d
        leal    (%edi,%r10d), %esi
        subq    $16, %rsp
        movq    %esi, $8(%rsp)
        movq    %r11d, (%rsp)
        call    sixth
        addq    $16, %rsp
        movl    %eax, %r11d
        leal    (%esi,%r11d), %r10d
        movl    %r10d, %eax
        ret