    .text
    .globl  scope
    .type   scope, @function
scope:
.Lscope_entry:
        movl    $0, %eax
        ret
.Lscope_end:
    .globl  scope_reassign_variable
    .type   scope_reassign_variable, @function
scope_reassign_variable:
.Lscope_reassign_variable_entry:
        movl    $7, %eax
        ret
.Lscope_reassign_variable_end:
    .globl  scope_reassign_in_parallel_scopes
    .type   scope_reassign_in_parallel_scopes, @function
scope_reassign_in_parallel_scopes:
.Lscope_reassign_in_parallel_scopes_entry:
        movl    $1, %eax
        ret
.Lscope_reassign_in_parallel_scopes_end:
    .section    .note.GNU-stack,"",@progbits
    .ident  "PANNICC (https://github.com/Golf0ned/PANNICC)"