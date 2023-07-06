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

#define NUM_SIZE 16
int main(void)
{
    long i = 0;
    long num[NUM_SIZE] = { 3, 23, 823, 823, 90823, 190823, 190823, 20190823 };
    for (i = 0; i < NUM_SIZE; i++) {
        printf("%10ld prime: %d.\n", num[i], isprime(num[i]));
    }
    for (i = 1; i <= 100; i++) {
        if (isprime(i)) {
            printf("%3ld,", i);
        }
    }
    printf("\n");
    return 0;
}
