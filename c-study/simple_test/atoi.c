#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#define NUMBER 10
#define NUMBER_TWO (NUMBER + 2)

int main()
{
    char temp_char[20],*p;
    uint32_t a;
    uint32_t b;
    uint32_t c;
    size_t ps = sysconf(_SC_PAGESIZE);

    a = 32; b = 0;
    c = b - a;
    printf("smart-tap:%lx\n", c); 
    printf("smart-tap:%lx\n", a - c); 
    unsigned int psize = sizeof (void (*) ()); 

    printf("psize is: %d.\n",psize);

    strcpy(temp_char,"1-3");
    p = strstr(temp_char,"-");
    printf("%d,%d\n",atoi(temp_char),atoi(p+1));
    
    psize = NUMBER;
    printf("********************number is %d\n",psize);
    psize = NUMBER_TWO;
    printf("********************number2 is %d\n",psize);
#undef NUMBER
#define NUMBER 9
    psize = NUMBER;
    printf("********************number is %d\n",psize);
    psize = NUMBER_TWO;
    printf("********************number2 is %d\n",psize);
    
    printf("\npage size is %d\n", ps);

    int m[17] = {7, 9, 10, 5, 8,
                 4, 2, 1, 6, 3,
                 7, 9, 10, 5, 8,
                 4, 2};
    int n[17] = {3, 1, 0, 1, 1,
                 0, 1, 9, 5, 7,
                 0, 5, 2, 0, 3,
                 2, 1};
    int nn[17] = {3, 7, 0, 7, 8,
                 4, 1, 9, 8, 3,
                 0, 6, 1, 2, 5,
                 8, 1};
    int sum = 0;
    int sum2 = 0;
    int i = 0;
    for (i = 0; i < 17; i++) {
        sum += m[i] * n[i];
        sum2 += m[i] * nn[i];
        }
        printf("sum is %d.\n", sum);
        printf("sum2 is %d.\n", sum2);

        int result = sum % 11;
        printf("result is %d.\n", result);

        result = sum2 % 11;
        printf("result is %d.\n", result);
    return 0;    
}

