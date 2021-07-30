
#include <stdio.h>
#include "b.h"

int a(void)
{
    int test;
    int *p = 0x0;
    b_inc();
    b_print();
    *p = 0x1234;
    printf("embedway debug test addr: %p, func a %p\n", &test, a);
    return 0;
}

