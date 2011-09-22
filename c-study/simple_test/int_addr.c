#include "stdio.h"
 
int main ()
{ 
   int i = 34;
   int *p;
   int j;
   p = &i;
   j = ( int )p;
   printf("the addr of i is 0x%x, 0x%x",p,j);
   return 0;
}

