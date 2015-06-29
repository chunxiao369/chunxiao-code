#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

void *myfun(void *arg)
{
    char buf[256];
    while (1) {
        memset(buf, 0, sizeof(buf));
    }
    pthread_exit(NULL);
}
/*
void *myfun(void *arg)
{
    cpu_set_t mask;
    cpu_set_t get;
    char buf[256];
    int i;
    int j;
    int num = sysconf(_SC_NPROCESSORS_CONF);
    printf("system has %d processor(s)\n", num);

    CPU_ZERO(&mask);
    for (i = 0; i < num; i++) {
        CPU_SET(i, &mask);
    }
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
        fprintf(stderr, "set thread affinity failed\n");
    }
    CPU_ZERO(&get);
    if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
        fprintf(stderr, "get thread affinity failed\n");
    }
    for (j = 0; j < num; j++) {
        if (CPU_ISSET(j, &get)) {
            printf("thread %d is running in processor %d\n", (int)pthread_self(), j);
        }
    }
    j = 0;
    //while (j++ < 100000000) {
    while (1) {
        memset(buf, 0, sizeof(buf));
    }
    pthread_exit(NULL);
}
*/
int main(int argc, char *argv[])
{
    int i;
    cpu_set_t mask;
    pthread_t tid[64];
    uint64_t core_mask = (~0ul);
    int num = sysconf(_SC_NPROCESSORS_CONF);

    if (argc >= 2) {
        core_mask = strtoul(argv[1], NULL, 16);
    }
    printf("system has %d processor(s), use 0x%lx\n", num, core_mask);
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
        if (pthread_create(&tid[i], NULL, (void *)myfun, NULL) != 0) {
            fprintf(stderr, "thread create failed\n");
            return -1;
        }
    }
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
        CPU_ZERO(&mask);
        CPU_SET(i, &mask);
        if (pthread_setaffinity_np(tid[i], sizeof(mask), &mask) < 0) {
            fprintf(stderr, "set thread affinity failed\n");
        }
    }
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
        pthread_join(tid[i], NULL);
    }
    return 0;
}
