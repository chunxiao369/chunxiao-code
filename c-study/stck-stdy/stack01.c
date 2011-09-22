
#include <stdio.h>
#include <stdint.h>
/*
int * fun()
{
    int a = 5;
    printf("%p\n", &a);
    return &a;
}
*/
int func()
{
    int b = 30;
    char *p;
    uint64_t a;
    
    a = 0x33UL;
    p = (char*)&a;
    for (b = 0; b < 8; b++)
        printf("%2x ", p[b]);
    //for ( b = 0; b <= 9; b++ )
    //    printf("b address: %p\n", &b);
    return b;
}
/*
void hehe(int *p1)
{
    printf("in hehe: %d\n", *p1);
}
*/
int main()
{
    //int *p = fun();
    
    func();    
    printf("return address: %p\n", func);
    //printf("main first: %d\n", *p);
    //printf("main second: %d\n", *p);
    //hehe(p);

    return 0;
}

