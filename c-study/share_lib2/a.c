
#include <stdio.h>
#include "b.h"

int a(void)
{
    int test;
    b_inc();
    b_print();
    printf("test addr: %p, func a %p\n", &test, a);
    return 0;
}

