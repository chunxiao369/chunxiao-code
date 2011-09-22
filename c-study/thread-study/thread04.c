
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <string.h> 

#define TNUM 2
typedef struct _BN
{
    pthread_t pid;
    int N;
} BN;

void *run(void *args)
{
    BN *recive = (BN *)args;
    printf("pid: %lu\n", recive->pid);
    printf("pid: %lu\n", pthread_self());
    printf("pid: %lu\n", gettid());
    return (void *)(recive->N);
}

int main(int c, char *v[])
{
    int i;
    int ret;
    pthread_t pid[TNUM] = {0, 0};
    BN bn[TNUM];

    for (i = 0; i < TNUM; i ++)
     {
        bn[i].pid = pid[i];
        bn[i].N = i;
        ret = pthread_create(&pid[i], NULL, run, &bn[i]);
        printf("pid in parent: %lu.\n", pid[i]);
    //...出错处理
    }
    while (1);
    return 0;
}

