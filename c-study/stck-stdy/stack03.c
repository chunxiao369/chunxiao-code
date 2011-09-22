#include <limits.h>
#include <pthread.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/*
 * test func return address is main return address
 */

void test_2()
{
    int a = 2;
    
    *(&a + 2) = *((int*)*(&a + 1) + 1);

    printf("abc, %d\n", 333);
}
void test()
{
    int a = 2;

    (int)*(&a + 2) = *((int*)*(&a + 1) + 1);

    //(int)*(&a + 2) = (int)test_2;//(int)*((int*)*(&a+1)+1);
    
    for ( a = 0; a < 2; a++ )
    {
        printf("abc, %d.\n", a);
    }
}

int main (int argc, char *argv[])
{
    test();
    printf("thread_routine address: \n" );
    
    return 0;
}


