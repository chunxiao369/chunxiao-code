#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

int main ()
{
    int a;
    int *p;
    int **q;

    p = &a;
    q = &p;
    p = NULL;
    *q = &a;
    printf("aaaaaaaaa.\n");
    return 0;
    int i = 0;
    uint64_t timestamp1;
    uint64_t timestamp2;
    struct timeval cur;

    gettimeofday( &cur, NULL );
    timestamp1 = cur.tv_sec * 1000000ull + cur.tv_usec * 1ull; 

    switch (i) {
    case 0:
        i++;
        break;
    case 1:
        i = i + 2;
        break;
    case 2:
        i = i + 3;
        break;
    default:
        break;
    }
    gettimeofday( &cur, NULL );
    timestamp2 = cur.tv_sec * 1000000ull + cur.tv_usec * 1ull; 
    printf("hello, world, %llu\n", timestamp2 - timestamp1);
    return 0;
}

