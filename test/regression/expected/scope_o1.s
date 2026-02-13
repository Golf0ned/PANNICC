scope:
.scope_entry:
        movl    $0, %eax
        ret
scope_reassign_variable:
.scope_reassign_variable_entry:
        movl    $7, %eax
        ret
scope_reassign_in_parallel_scopes:
.scope_reassign_in_parallel_scopes_entry:
        movl    $1, %eax
        ret