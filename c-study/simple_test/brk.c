#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
int bss_end;

int main(void)
{
#if 1        
    void *tret;
    printf("bss end: %p\n", (char *)(&bss_end) + 4);
    tret = sbrk(0);
    if (tret != (void *)-1)
        printf ("heap start: %p\n", tret);
    return 0;
#endif
#if 0        
    char *pmem;

    printf("bss end: %p\n", (char *)(&bss_end) + 4);
    tret = sbrk(0);
    if (tret != (void *)-1)
        printf ("heap1 start: %p\n", tret);

    if (brk((char *)tret - 1) == -1)
        printf("brk error\n");

    tret = sbrk(0);
    if (tret != (void *)-1)
        printf ("heap2 start: %p\n", tret);

    pmem = (char *)malloc(32);
    if (pmem == NULL) {
        perror("malloc");
        exit (EXIT_FAILURE);
    }
    printf ("pmem:%p\n", pmem);

    tret = sbrk(0);
    if (tret != (void *)-1)
        printf ("heap1 end: %p\n", tret);

    if (brk((char *)tret - 10) == -1)
        printf("brk error\n");

    tret = sbrk(0);
    if (tret != (void *)-1)
        printf ("heap2 end: %p\n", tret);
    return 0;
#endif
}
