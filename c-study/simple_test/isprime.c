#include <stdio.h>

int isprime(long num)
{
    long i = 3;
    if (num == 2) {
        return 1;
    }
    if (num % 2 == 0) {
        return 0;
    } else {
        while (i * i <= num) {
            if (num % i == 0)
                return 0;
            else
                i += 2;
        }
        return 1;
    }
}

int main(void)
{
    long i = 0;
    long num[16] = {3,23,823,823,90823,190823,190823,20190823};
    for (i = 0; i <= 15; i++) {
        printf("%ld prime: %d.\n", num[i], isprime(num[i]));
    }
    return 0;
}
