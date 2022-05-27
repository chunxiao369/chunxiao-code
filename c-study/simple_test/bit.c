#include "stdio.h"
#include <stdint.h>

static inline uint64_t __align64pow2(uint64_t v) {
    v--;
    //printf("0x%lx\n", v);
    v |= v >> 1;
    //printf("0x%lx\n", v);
    v |= v >> 2;
    //printf("0x%lx\n", v);
    v |= v >> 4;
    //printf("0x%lx\n", v);
    v |= v >> 8;
    //printf("0x%lx\n", v);
    v |= v >> 16;
    //printf("0x%lx\n", v);
    v |= v >> 32;
    //printf("0x%lx\n", v);
    return v + 1;
}
/* 就是把连同最高位之后的所有的bit都变为1，最后由二进制若干个1加上1就变成1和若干个0。
 * 一种情况是数本身就是2的幂次方，这时就先-1，再去执行此操作。
 */

int main()
{
    unsigned int a = 3;
    unsigned int b = ~(a & 0xff);
    uint16_t ptr;
    uint16_t *ptr_16;
    int i;
    int j;
    uint64_t array[16][5];
    printf("%lu, 0x%lx, %d\n", __align64pow2(10), __align64pow2(10), 10);
    
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


