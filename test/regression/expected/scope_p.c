int scope() {
    int before = 0;
    {
        int before = 1;
        {
            int before = 2;
            {
                int in = before + 3;
            }
            int after = 4;
        }
        int after = 5;
        after = after + before;
    }
    int after = 6;

    return 0;
}

int scope_reassign_variable() {
    int cond = 1;
    {
        cond = cond + 1;
        int cond = 3;
        cond = cond + 4;
    }
    cond = cond + 5;
    return cond;
}

int scope_reassign_in_parallel_scopes() {
    int typed = 1;
    {
        int typed = 2;
    }
    {
        int typed = 3;
    }
    return typed;
}
