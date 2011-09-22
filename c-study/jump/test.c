#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <curses.h>

#include   <stdio.h>
//#include   <process.h>
#include   <setjmp.h>

void subroutine(void);
jmp_buf jumper;
extern void subroutine_module1(void);

int main(void)
{
    int value;
    static int i = 0;

    printf("About   to   call   subroutine   ...   \n");
    value = setjmp(jumper);
    if (value != 0) {
        printf("Longjmp   with   value   %d\n", value);
    }
    while (1) {
        i++;
        if ( i % 2 == 0) {
            subroutine_module1();
        } else {
            subroutine();
        }
    }
    return 0;
}

void subroutine(void)
{
    longjmp(jumper, 1);
}
