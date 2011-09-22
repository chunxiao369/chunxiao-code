#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int func(int **nie_p, int *size, int num)
{
    int all = 1;
    int temp_int;
    int i;
    int j;
    int *temp;
    int *position;

    for (i = 0; i < num; i++) {
        all *= size[i];
    }
    printf("all is %d.\n", all);

    temp = malloc(num *sizeof(int));
    position = malloc(num *sizeof(int));
    temp_int = all;
    for (i = 0; i < num; i++) {
        temp[i] = temp_int / size[i];
        temp_int = temp_int / size[i];
    }
    for (i = 0; i < num; i++) {
        printf("temp[ %d ] is %d .\n", i, temp[i]);
    }

    for (i = 0; i < all; i++) {
        for (j = 0; j < num; j++) {
            position[j] = i / temp[j];
            position[j] %= size[j];
        }
        /*
        for (j = 0; j < num; j++) {
            printf("%d  ",  position[j]);
        }
        */
        for (j = 0; j < num; j++) {
            printf("%d  ", nie_p[ j ][ position[j] ]);
        }
        printf("\n");
    }
    free(temp);
    free(position);
    return 0;
}

int main()
{
    int value1[]={1, 2, 3, 4, 5};
    int value2[]={1, 2, 3, 4, 5, 6};
    int value3[]={1, 2, 3, 4, 5, 6, 7};
    int value4[]={1, 2, 3, 4, 5, 6, 7, 8};
    
    int *value[]={value1, value2, value3,  value4};
    int len[]={5, 6, 7, 8};
   
    func(value, len, sizeof(value) / sizeof(int *)); 
    
    return 0;
}
