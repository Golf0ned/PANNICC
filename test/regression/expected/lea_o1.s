leaBI:
        movl    %edi, %r11d
        movl    %esi, %r10d
.LleaBI_entry:
        leal    (%r11d,%r10d), %r10d
        movl    $0, %eax
        ret
leaIS:
        movl    %edi, %r10d
.LleaIS_entry:
        movl    %r10d, %r10d
        leal    (,%r10d,2), %r10d
        leal    (,%r10d,4), %r10d
        leal    (,%r10d,8), %r10d
        movl    %r10d, %r10d
        leal    (,%r10d,2), %r10d
        leal    (,%r10d,4), %r10d
        leal    (,%r10d,8), %r10d
        movl    $0, %eax
        ret
leaBIS:
        movl    %edi, %r11d
        movl    %esi, %r10d
.LleaBIS_entry:
        leal    (%r11d,%r10d,2), %r10d
        leal    (%r11d,%r10d,2), %r10d
        leal    (%r11d,%r10d,2), %r10d
        leal    (%r11d,%r10d,2), %r10d
        movl    $0, %eax
        ret
leaBID:
        movl    %edi, %r10d
        movl    %esi, %r11d
.LleaBID_entry:
        leal    $13(%r10d,%r11d), %r10d
        leal    $13(%r10d,%r11d), %r10d
        leal    $13(%r10d,%r11d), %r10d
        leal    $13(%r11d,%r10d), %r10d
        leal    $13(%r11d,%r10d), %r10d
        leal    $13(%r10d,%r11d), %r10d
        movl    $0, %eax
        ret
leaIIS:
        movl    %edi, %r10d
.LleaIIS_entry:
        leal    (%r10d,%r10d,2), %r10d
        leal    (%r10d,%r10d,4), %r10d
        leal    (%r10d,%r10d,8), %r10d
        leal    (%r10d,%r10d,2), %r10d
        leal    (%r10d,%r10d,4), %r10d
        leal    (%r10d,%r10d,8), %r10d
        movl    $0, %eax
        ret
leaISD:
        movl    %edi, %r10d
.LleaISD_entry:
        leal    $60(,%r10d,4), %r10d
        leal    $60(,%r10d,4), %r10d
        leal    $60(,%r10d,4), %r10d
        leal    $60(,%r10d,4), %r10d
        movl    $0, %eax
        ret
leaIISD:
        movl    %edi, %r10d
.LleaIISD_entry:
        leal    $60(%r10d,%r10d,2), %r10d
        leal    $60(%r10d,%r10d,4), %r10d
        leal    $60(%r10d,%r10d,8), %r10d
        leal    $60(%r10d,%r10d,2), %r10d
        movl    $0, %eax
        ret
leaBISD:
        movl    %edi, %r11d
        movl    %esi, %r10d
.LleaBISD_entry:
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        leal    $40(%r11d,%r10d,8), %r10d
        movl    $0, %eax
        ret