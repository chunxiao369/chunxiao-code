
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <string.h> 

void *run(void *args)
{
    pthread_exit(NULL);
}

int test();
int main(int c, char *v[])
{
    int i = 0;
    int ret = 0;
    while (1) {
        usleep(1);
        ret = test(); 
        if (i % 1000 == 0) {
            printf("i: %d.\n", i);
        }
        if (ret) {
            break;
        }
        i++;
    }
    printf("i: %d.\n", i);
    printf("ret: %d.\n", ret);
    return 0;
}

int test()
{
    int ret;
    pthread_t pid;
    pthread_attr_t attr;

    //pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    //ret = pthread_create(&pid, &attr, run, NULL);
    ret = pthread_create(&pid, NULL, run, NULL);
    //pthread_attr_destroy(&attr);
    if (ret) {
        return ret;
    }
    return 0;
}

