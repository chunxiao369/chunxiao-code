
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int max_find(int *array, int size)
{
    int i;
    int max = array[0];

    for (i = 1; i < max; i++) {
        if(max < array[i])
            max = array[i];
    }

    return max;
}

void sort_func(int *array, int size)
{
    int max;
    int *p;
    int i;
    int j;

    max = max_find(array, size);
    p = malloc(size * sizeof(int));

    memset(p, 0, (size + 1) * sizeof(int));
    for (i = 0; i < size; i++) {
        p[array[i]]++;
    }
    
    j = 0;
    for (i = 1; i <= max; i++) {
        while (p[i]) {
            array[j] = i;
            j++;
            p[i]--;
        };
    }   
}

void print_array(int *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main()
{
    int array[6] = {5, 3, 2, 1, 5, 6};
    
    print_array(array, 6);
    sort_func(array, 6);    
    print_array(array, 6);
    
    return 0;
}

