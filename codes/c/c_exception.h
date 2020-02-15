
#pragma once

#if __cplusplus
extern "C"
{
#endif

    typedef void (*test_exception)();
    struct fn_table
    {
        test_exception throwException;
    };
    fn_table *test2(test_exception t)
    {
        fn_table *pte = new fn_table;
        pte->throwException = t;
        return pte;
    }
    struct evmc_context
    {
        /** Function table defining the context interface (vtable). */
        const struct fn_table *fn_table;
    };

#if __cplusplus
}
#endif