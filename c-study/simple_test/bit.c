#include "stdio.h"
#include <stdint.h>

static inline uint64_t __align64pow2(uint64_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return v + 1;
}

int main()
{
    unsigned int a = 3;
    unsigned int b = ~(a & 0xff);
    uint16_t ptr;
    uint16_t *ptr_16;
    int i;
    int j;
    uint64_t array[16][5];
    
    printf("%lu, 0x%lx, %d\n", __align64pow2(33), __align64pow2(33), 33);
    printf("%lu, 0x%lx, %d\n", __align64pow2(0), __align64pow2(0), 0);
    printf("%x",b);
    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 5; j++)
            array[i][j] = 0x0123456789ABCDEFull;
    }
    ptr_16 = (uint16_t*)array[3];
    for (i = 0; i < 4; i++)
    {
        ptr = *ptr_16;
        printf("%x\n",ptr);
        ptr_16++;
    }
    return 0;
}


