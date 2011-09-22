#include <stdio.h>

typedef struct _example{
    char  a;
    short b;
    int   c;
}example_t;

example_t test_ex[] = {
{'a', 2, 3},
{'b', 3, 4},
{'c', 4, 5}
};

#define LENG_TEST_A (sizeof(test_ex) / sizeof(example_t))
#define LENG_TEST_B (sizeof(test_ex) / sizeof(test_ex[0]))

int main()
{
    printf("temperature: 33 \n");
    printf(" LENG_TEST_A: %d\n", LENG_TEST_A);
    printf(" LENG_TEST_B: %d\n", LENG_TEST_B);

    return 0;
}

