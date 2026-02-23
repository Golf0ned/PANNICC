square:
        movl    %edi, %r11d
.Lsquare_entry:
        movl    %r11d, %r10d
        imull   %r11d, %r10d
        movl    %r10d, %eax
        ret