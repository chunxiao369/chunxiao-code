#include <stdio.h>
#include <stdint.h>

#if 0
int main()
{
   // find the number of trailing zeros in 32-bit v 
   unsigned int v;
   int r; // result goes here
    v = 0x3000;
   static const int MultiplyDeBruijnBitPosition[32] =                                           
   {                                                                                                                     
   0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,                                                           
   31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9                                                          
   };
   
    r = MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27];
    printf("r is %d.\n", r);
    return 0;
}
#endif

reverse1(unsigned int x)
{  
    register unsigned int y = 0x55555555;  
    x = (((x>> 1) & y) | ((x & y)<< 1));
    y = 0x33333333;  
    x = (((x>> 2) & y) | ((x & y)<< 2));
    y = 0x0f0f0f0f;  
    x = (((x>> 4) & y) | ((x & y)<< 4));
    y = 0x00ff00ff;  
    x = (((x>> 8) & y) | ((x & y)<< 8));
    return((x>> 16) | (x<< 16));
}  

reverse2(register unsigned int x)  
{
#if 0
    __asm__ {
        mov cx,32;
        mov eax,x;
        labNext:
        rcl eax,1;
        rcr ebx,1;
        loop labNext;
       mov eax,ebx;
    }
#endif
#if 0
    __asm__ __volatile__(
          "mov cx,32"
     :    "mov eax,(x)"
        labNext:
     :    "rcl eax,1"
     :    "rcr ebx,1"
     :    "loop labNext"
     :    "mov eax,ebx"
    );
#endif
}  

int main()  
{
    register unsigned int x = 0;

    printf("reverse1()");
    x = reverse1(2147483640);
    printf("x is 0x%x\n", x);

    printf("reverse2()");
    x = reverse2(2147483640);
    printf("x is 0x%x\n", x);
    return 0;
}

