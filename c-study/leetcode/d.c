
#include <stdio.h>
#include <stdint.h>

int is_prime_factor(int num)
{
    int i;
    if (num == 2)
        return 0;
    if (num == 3)
        return 1;
    for (i = 2; i <= num / 2; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main()
{
    //uint64_t NUM = 13195;//600851475143ull;
    uint64_t NUM = 600851475143llu;
    //uint64_t NUM = 8;
    uint64_t i = 2;
    uint64_t max = 0;
    uint64_t a;
    uint64_t b = NUM;
    while (i <= b) {
        a = b%i;
        if (a) {
            i++;
            continue;
        }
        printf("%llu\n", i);
        b = b / i;
        if (i > max)
            max = i;
    }
        
    printf("aaaaaaaaaaaaa, max: %llu.\n", max);
    return 0;
}

