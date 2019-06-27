
#include <stdio.h>

int cxxu_global = 0;
int b(void)
{
    int test;
    printf("test addr: %p\n", &test);
    return 0;
}

int b_inc(void)
{
    cxxu_global++;
    return 0;
}

int b_get_global(void)
{
    return cxxu_global;
}
