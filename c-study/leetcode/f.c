
#include <stdio.h>
#include <stdint.h>

int is_palindromic_factor(int num)
{
    char buf[64] = {0};
    int i;
    int len;
    sprintf(buf, "%d", num);
    len = strlen(buf);
    for (i = 0; i < len / 2; i++) {
        if (buf[i] != buf[len - 1 - i])
            return 0;
    }
    return 1;
}

int main()
{
    //uint64_t NUM = 13195;//600851475143ull;
    //uint64_t NUM = 8;
    uint64_t i = 2;
    int a = 999; 
    int b = 999;
    int m;
    int max;
    while ( a > 100 && b > 100) {
        if (is_palindromic_factor(a * b))
            break;
        a--;
        if (is_palindromic_factor(a * b))
            break;
        b--;
    }
    i = 0;
    max = 836 * 836;
    for (a = 999; a >= 836; a--) {
        for (b = 999; b >= 836; b--) {
            m = a * b;
            if (is_palindromic_factor(m)) {
                if (m > max) {
                    max = m;
                    printf("a: %d, b: %d, %d.\n", a, b, max);
                }
            }
        }
    }
    printf("aaaaaaaaaaaaa, a: %d, b: %d, %d.\n", a, b, max);
    return 0;
}

