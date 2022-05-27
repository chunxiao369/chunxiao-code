
#include <stdio.h>
int main()
{
    //多少个1
    printf("buildin: %d.\n", __builtin_popcount(0xf));
    printf("buildin: %d.\n", __builtin_popcountll(0xf0000000f));

    //二进制中1的个数的奇偶性
    printf("buildin: %d.\n", __builtin_parity(0xf));
    printf("buildin: %d.\n", __builtin_parity(0x1f));

    //二进制末尾最后一个1的位置
    printf("buildin: %d.\n", __builtin_ffs(0x1f));
    printf("buildin: %d.\n", __builtin_ffs(0x10));

    //二进制末尾后面0的个数
    printf("buildin: %d.\n", __builtin_ctz(0x100));
    printf("buildin: %d.\n", __builtin_ctz(0xf0));
    printf("buildin: %d.\n", __builtin_ctzll(0xf00000000));

    //二进制前面0的个数
    printf("buildin: %d.\n", __builtin_clz(0x100));
    printf("buildin: %d.\n", __builtin_clzll(0xf00000000));
    return 0;
}

