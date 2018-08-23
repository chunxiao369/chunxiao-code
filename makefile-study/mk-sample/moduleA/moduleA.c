#include <stdio.h>
int g_a = 0;

static int __ga_xx(void)
{
    return 0;
}

int func_a()
{
    g_a = 0;
    g_a = g_a;
    __ga_xx();
	printf("in funca\n");
	return 0;
}
