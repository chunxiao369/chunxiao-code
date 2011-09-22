
#include   <setjmp.h>

extern jmp_buf jumper;
void subroutine_module1(void)
{
    longjmp(jumper, 3);
}
