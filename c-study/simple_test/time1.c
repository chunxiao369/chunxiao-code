#include <stdio.h>
#include <time.h>
typedef struct _sccp {
    int a;
    int b;
    int c;
} sccp_t;

sccp_t * function()
{
    sccp_t temp;
    
    temp.a = 6;
    temp.b = 7;
    temp.c = 8;

    return &temp;
}
int function_2()
{
    int a;
    int b;
    int c;
    
    a = 9;
    b = 10;
    c = 11;
    
    return a;
}
/*
    test for predefined macro
*/
int main()
{
    time_t biggest;
    time_t middle;
    time_t smallest;
    sccp_t *temp;
    printf("##############date: %s, time: %s\n", __DATE__, __TIME__);
    printf("##############date: %s, time: %d\n", "aa", __LINE__);
    printf("##############file: %s, func: %s\n", __FILE__, __FUNCTION__);
    //printf("sizeof time_t: %d\n", sizeof(time_t));
    biggest  = 0x7fffffff;
    middle   = 0xffffffff;
    smallest = 0x80000000;
    
    printf("biggest = %s", ctime(&biggest));
    printf("biggest = %s", asctime(gmtime(&biggest)));
    
    printf("middle = %s", ctime(&middle));
    middle = 0;
    printf("middle = %s", ctime(&middle));

    printf("smallest = %s", ctime(&smallest));
    
    temp = function();
    function_2();
    printf("a: %d, b: %d, c: %d.\n",temp->a, temp->b, temp->c);
    return 0;
}

