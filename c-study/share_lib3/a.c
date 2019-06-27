
#include <stdio.h>

int cxxu_global = 0;
int a(void)
{
    int test;
    printf("test addr: %p\n", &test);
    return 0;
}

int inc(void)
{
    cxxu_global++;
    return 0;
}

int get_global(void)
{
    return cxxu_global;
}
