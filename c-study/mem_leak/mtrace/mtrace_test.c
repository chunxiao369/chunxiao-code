#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

//#define FIRST_TEST
#define SECOND_TEST
void *t_malloc()
{
    void *p;
    p = malloc(20);
    return p;
}

int main()
{
    setenv("MALLOC_TRACE", "output.log", 1);
    mtrace();
#ifdef SECOND_TEST
    void *p;
    int i = 0;

    while (i < 20) {
        p = t_malloc();
        if (i % 2)
            free(p);
        i++;
    }
    printf("hello: 0x%08x\n", *(int*)p);
#endif
#ifdef FIRST_TEST
    char *hello;
    hello = (char *)malloc(20);
    sprintf(hello, "\nhello world!");
    printf("hello: %s\n", hello);
#endif
    return 1;
}


