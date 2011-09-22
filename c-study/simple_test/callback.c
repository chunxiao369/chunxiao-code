
#include <stdlib.h>
#include <stdio.h>
int Test1()
{
    int i;
    for (i=0; i<30; i++)
    {
        printf("The %2d th charactor is: %c\n", i, (char)('a' + i%26));
    }
    return 0;
}

int Test2(int num)
{
    int i;
    for (i=0; i<num; i++)
    {
        printf("The %2d th charactor is: %c\n", i, (char)('a' + i%26));
    }
    return 0;
}

void Caller1(int (*ptr)())//............
{
    (*ptr)();
}

void Caller2(int n, int (*ptr)())//............,....................
{ //....void Caller2(int (*ptr)(int n))...........
    (*ptr)(n);
    return;
}

int main()
{
    printf("************************\n");
    Caller1(Test1); //.....Test2();
    printf("&&&&&&************************\n");
    Caller2(30, Test2); //.....Test2(30);
    return 0;
}

