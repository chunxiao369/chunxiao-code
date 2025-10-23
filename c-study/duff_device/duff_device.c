// C program to illustrate the working of
// Duff's Device. The program copies given
// number of elements bool array src[] to
// dest[]
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>

// Copies size bits from src[] to dest[]
void copyBoolArray(bool src[], bool dest[],
                               int size)
{
    // Do copy in rounds of size 8.
    int rounds = size / 8;

    int i = 0;
    switch (size % 8)
    {
    case 0:
        while (!(rounds == 0))
        {
            rounds = rounds - 1;
            dest[i] = src[i];
            i = i + 1;

        // An important point is, the switch
        // control can directly reach below labels
        case 7:
            dest[i] = src[i];
            i = i + 1;
        case 6:
            dest[i] = src[i];
            i = i + 1;
        case 5:
            dest[i] = src[i];
            i = i + 1;
        case 4:
            dest[i] = src[i];
            i = i + 1;
        case 3:
            dest[i] = src[i];
            i = i + 1;
        case 2:
            dest[i] = src[i];
            i = i + 1;
        case 1:
            dest[i] = src[i];
            i = i + 1;
        };
    }
}

// Driver code
int main()
{
    int size = 20;
    bool dest[size], src[size];

    // Assign some random values to src[]
    int i;
    for (i=0; i<size; i++)
        src[i] = rand()%2;

    copyBoolArray(src, dest, size);

    for (i=0; i<size ; i++)
        printf("%d\t%d\n", src[i], dest[i]);
}
