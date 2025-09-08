int64_t scope() {
    int64_t before = 0;
    {
        int64_t before = 1;
        {
            int64_t before = 2;
            {
                int64_t in;
                in = before + 3;
            }
            int64_t after = 4;
        }
        int64_t after = 5;
        after = after + before;
    }
    int64_t after = 6;

    return 0;
}
