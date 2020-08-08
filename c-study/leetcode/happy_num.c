#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int happy(int num)
{
    int b;
    int c;
    int sum = 0;
    int digits = 0;
    int a = num;

    for (b  = a; b; b = b/10) {
        digits++;
    }
    int *p = malloc((digits * 81) * sizeof(int));
    memset((void *)p, 0, (digits * 81) * sizeof(int));
    while (1) {
        sum = 0;
        for (b  = a; b; b = b/10) {
            c = b % 10;
            sum += c*c;
        }
        printf("sum: %d\n", sum);
        if (sum == 1) {
            break;
        } else {
            if (1 == p[sum]) {
                return 0;
            }
            p[sum] = 1;
            a = sum;
        }
    }
    free(p);
    return 1;
    
}

int main(void)
{
    happy(2);
    return 0;
}
