
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

extern int inc(void);
extern int b_inc(void);
extern int get_global(void);
extern int cxxu_global;
int main(int argc, char *argv[])
{
    int counter = 0;
    //int cxxu_global;
    cxxu_global = 2;
    b_inc();
    /*
    inc();
    while (1) {
        if (get_global() != 1) {
            printf("2 global: %d.\n", get_global());
            break;
        } else {
            counter++;
        }
    }
    */
    printf("2 counter: %d, %d.\n", counter, cxxu_global);
    return 0;
}
