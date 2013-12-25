#ifndef __cplusplus
/* Build-time assertion for use in a declaration context. */
#define EWX_ASSERT(EXPR)                                                \
        extern int (*build_assert(void))[ sizeof(struct {               \
                    unsigned int build_assert_failed : (EXPR) ? 1 : -1; })]
#else /* __cplusplus */
#define EWX_ASSERT(_EXPR) typedef int build_assert_failed[(_EXPR) ? 1 : -1]
#endif /* __cplusplus */                

typedef struct _test {
    int a;
    int b;
} test_t;

EWX_ASSERT(sizeof(test_t) == 8);
int main()
{
    return 0;
}
