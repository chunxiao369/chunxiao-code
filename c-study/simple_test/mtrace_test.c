#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

#define SECOND_TEST
void *t_malloc()
{
    void *p;
    p = malloc(20);
    return p;
}

int main()
{
#ifdef SECOND_TEST
    void *p;
    int i = 0;
    mtrace();

    while (i < 20) {
        p = t_malloc();
        if (i % 2)
            free(p);
        i++;
    }
#endif
#ifdef FIRST_TEST
    char *hello;
    mtrace();
    hello = (char *)malloc(20);
    sprintf(hello, "\nhello world!");
#endif
    return 1;
}

/* test record
---------------FIRST_TEST----------------------
#make
#export MALLOC_TRACE=xu.log
#./xuexe
#mtrace xuexe xu.log 

Memory not freed:
-----------------
           Address     Size     Caller
0x00000000006d6460     0x14  at 0x400473

---------------SECOND_TEST----------------------
#make
#export MALLOC_TRACE=xu.log
#./xuexe
#mtrace xuexe xu.log 

Memory not freed:
-----------------
           Address     Size     Caller
0x000000000130e460     0x14  at 0x4004af
0x000000000130e480     0x14  at 0x4004bf
0x000000000130e4a0     0x14  at 0x4004bf
0x000000000130e4c0     0x14  at 0x4004bf
0x000000000130e4e0     0x14  at 0x4004bf
0x000000000130e500     0x14  at 0x4004bf
0x000000000130e520     0x14  at 0x4004bf
0x000000000130e540     0x14  at 0x4004bf
0x000000000130e560     0x14  at 0x4004bf
0x000000000130e580     0x14  at 0x4004bf

*/
