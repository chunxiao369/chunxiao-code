#include <stdio.h>
#include <math.h>

double f(double);

double f(double a)
{
    return (4.0 / (1.0 + a * a));
}

int main(void)
{
    double x, h, mypi, sum;
    int i, n = 1000;
    h = 1.0 / (double) n;
    sum = 0.0;
#if 0
    sum = 0.0;
    for (i = 1; i <= n; i += 1) {
        sum += h * f((i - 0.5)/n);
    }
    printf("sum pai: %f\n", sum);
#endif
#if 1
    sum = 0.0;
    for (i = 1; i <= n; i += 1) {
        x = h * ((double) i - 0.5);
        sum += f(x);
    }
    mypi = h * sum;
#endif
    printf("pai: %f\n", mypi);
    return 0;
}

