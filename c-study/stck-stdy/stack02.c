#include <limits.h>
#include <pthread.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/*
 * test func return address is main return address
 */
/*
void test_2()
{
    int a = 2;
    
    (int)*(&a + 2) = 0x80482e1;

    printf("xuchunxiao.\n");
}
*/
void test()
{
    int a = 2;

    //(int)*(&a + 1) = (int)*(&a + 2);

    //(int)*(&a + 9) = (int)*((int *)*(&a + 1) + 1); 
    //(int)*(&a + 8) = *(int *)*(&a + 1); 
    (int)*(&a + 6) = (int)*((int *)*(&a + 1) + 1); 
    //(int)*(&a + 6) = (int)*(&a + 1);
    
    (int)*(&a + 2) = (int)printf;//(int)*((int*)*(&a+1)+1);
    (int)*(&a + 3) = 2;
    (int)*(&a + 4) = (int)("abc, %d.\n");
    (int)*(&a + 5) = 5;    
    
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


