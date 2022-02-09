#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define ARRAY_SIZE 8192 

int func_a(int num)
{
    int i;
    int j;
    uint8_t *a = NULL;

    a = (uint8_t *)malloc(ARRAY_SIZE * ARRAY_SIZE);
    if (NULL == a) {
        printf("malloc failed.\n");
        return 0;
    }
    if (1 == num) {
        for (i = 0; i < ARRAY_SIZE; ++i) {
            for (j = 0; j < ARRAY_SIZE; ++j) {
                a[i * ARRAY_SIZE + j] = 2;
            }
        }
    } else {
        for (i = 0; i < ARRAY_SIZE; ++i) {
            for (j = 0; j < ARRAY_SIZE; ++j) {
                a[j * ARRAY_SIZE + i] = 2;
            }
        }
    }
    free(a);
    return 0;
}

int func_b(void)
{
    int i;
    i = atoi("333");
    i = i + i;
    return i;
}

int main(int argc, char **argv)
{
    func_a(argc);
    func_b();
    return 0;
}