
#include <stdio.h>
#include <stdint.h>
int main()
{
    uint64_t a = 4000000;
    uint64_t i = 1;
    uint64_t j = 2;
    uint64_t t;
    uint64_t sum = j;
    while (1) {
        t = j;
        j = i + j;
        if ( j >= a) {
            break;
        }
        if (j % 2 == 0) {
            //printf("%lld ", j);
            sum += j;
        }
        i = t;
    }
    printf("aaaaaaaaaaaaa, sum: %llu.\n", sum);
    return 0;
}

