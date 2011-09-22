
#include <stdio.h>

int func()
{
    int a = 2;
    
    *(&a + 2) = *((int *)*(&a + 1) + 1);
    printf("abc, %d\n", a); 
    return 0;
}
int main()
{
    func();
    printf("def\n");
    return 0;
}

