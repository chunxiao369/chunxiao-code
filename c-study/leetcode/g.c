
#include <stdio.h>
#include <stdint.h>

int is_prime_factor(int num)
{
    int i;
    if (num == 2)
        return 1;
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
    int m[20];
    int i;
    int j;
    memset(m, 0, sizeof(m));
    int max = 1;
    for (i = 1; i <= 20; i++) {
       printf("%d.\n", max);
       if (max % i == 0)
           continue;
       else {
           if (is_prime_factor(i)) {
               max *= i;
           } else {
               for (j = 2; j <= i / 2; j++) {
                   if ((max * j) % i == 0) {
                       max *= j;
                       break;
                   }
               }
           }
       }
    }
    printf("aaaaaaaaaaaaa,  %d.\n", max);
    return 0;
}

