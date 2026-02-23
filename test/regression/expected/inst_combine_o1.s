constant_fold:
.Lconstant_fold_entry:
        movl    $55, %eax
        ret
constant_fold_overflow:
.Lconstant_fold_overflow_entry:
        movl    $18446744071562067968, %eax
        ret
constant_fold_underflow:
.Lconstant_fold_underflow_entry:
        movl    $2147483647, %eax
        ret