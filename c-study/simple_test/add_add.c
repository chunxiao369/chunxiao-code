
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <signal.h>

#define CHUN
int main()
{
    int a = 0;
    int b = (++a) + (++a);
    uint32_t para1; 
    uint32_t para2;
#ifdef CHUN
    printf("b is %d.\n", b);
#endif
    para1 = 1; 
    para2 = 2;

    if ((int)(para1 - para2) >= 0) {
        printf("b is %d.\n", b);
    }
    while (1) {
        kill(getpid(), SIGHUP);
    }
    return 0;
}

