    .text
    .globl  leaBI
    .type   leaBI, @function
leaBI:
        movl    %edi, %r11d
        movl    %esi, %r10d
.LleaBI_entry:
        leal    (%r11d,%r10d), %r10d
        movl    $0, %eax
        ret
.LleaBI_end:
    .globl  leaIS
    .type   leaIS, @function
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
.LleaIS_end:
    .globl  leaBIS
    .type   leaBIS, @function
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
.LleaBIS_end:
    .globl  leaBID
    .type   leaBID, @function
leaBID:
        movl    %edi, %r10d
        movl    %esi, %r11d
.LleaBID_entry:
        leal    13(%r10d,%r11d), %r10d
        leal    13(%r10d,%r11d), %r10d
        leal    13(%r10d,%r11d), %r10d
        leal    13(%r11d,%r10d), %r10d
        leal    13(%r11d,%r10d), %r10d
        leal    13(%r10d,%r11d), %r10d
        movl    $0, %eax
        ret
.LleaBID_end:
    .globl  leaIIS
    .type   leaIIS, @function
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
.LleaIIS_end:
    .globl  leaISD
    .type   leaISD, @function
leaISD:
        movl    %edi, %r10d
.LleaISD_entry:
        leal    60(,%r10d,4), %r10d
        leal    60(,%r10d,4), %r10d
        leal    60(,%r10d,4), %r10d
        leal    60(,%r10d,4), %r10d
        movl    $0, %eax
        ret
.LleaISD_end:
    .globl  leaIISD
    .type   leaIISD, @function
leaIISD:
        movl    %edi, %r10d
.LleaIISD_entry:
        leal    60(%r10d,%r10d,2), %r10d
        leal    60(%r10d,%r10d,4), %r10d
        leal    60(%r10d,%r10d,8), %r10d
        leal    60(%r10d,%r10d,2), %r10d
        movl    $0, %eax
        ret
.LleaIISD_end:
    .globl  leaBISD
    .type   leaBISD, @function
leaBISD:
        movl    %edi, %r11d
        movl    %esi, %r10d
.LleaBISD_entry:
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        leal    40(%r11d,%r10d,8), %r10d
        movl    $0, %eax
        ret
.LleaBISD_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"