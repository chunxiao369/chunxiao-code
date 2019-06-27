
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

extern int inc(void);
extern int get_global(void);
extern int global;
int main(int argc, char *argv[])
{
    inc();
    global++;
    printf("global: %d.\n", get_global());
    while(1) {
        global++;
    }
    return 0;
}
