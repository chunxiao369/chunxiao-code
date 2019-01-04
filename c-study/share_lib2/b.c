
#include <stdio.h>

int g_var = 0;
int b_print(void)
{
    int btest = 0;
    btest = btest;
    printf("g_var is %d\n", g_var);
    return 0;
}

int b_inc()
{
    g_var++;
    return 0;
}


