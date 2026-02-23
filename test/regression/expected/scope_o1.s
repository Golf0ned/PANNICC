scope:
.Lscope_entry:
        movl    $0, %eax
        ret
scope_reassign_variable:
.Lscope_reassign_variable_entry:
        movl    $7, %eax
        ret
scope_reassign_in_parallel_scopes:
.Lscope_reassign_in_parallel_scopes_entry:
        movl    $1, %eax
        ret