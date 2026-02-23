constant_fold:
.Lconstant_fold_entry:
.Lconstant_fold_0:
        movl    $55, %eax
        ret
constant_fold_overflow:
.Lconstant_fold_overflow_entry:
.Lconstant_fold_overflow_0:
        movl    $18446744071562067968, %eax
        ret
constant_fold_underflow:
.Lconstant_fold_underflow_entry:
.Lconstant_fold_underflow_0:
        movl    $2147483647, %eax
        ret