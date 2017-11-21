#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
pthread_mutex_t mutex_1;
pthread_mutex_t mutex_2;

void *child1(void *arg)
{
    while (1) {
        pthread_mutex_lock(&mutex_1);
        sleep(3);
        pthread_mutex_lock(&mutex_2);
        printf("thread 1 get running\n");
        pthread_mutex_unlock(&mutex_2);
        pthread_mutex_unlock(&mutex_1);
        sleep(5);
    }
}

void *child2(void *arg)
{
    while (1) {
        pthread_mutex_lock(&mutex_2);
        pthread_mutex_lock(&mutex_1);
        printf("thread 2 get running\n");
        pthread_mutex_unlock(&mutex_1);
        pthread_mutex_unlock(&mutex_2);
        sleep(5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;

    pthread_mutex_init(&mutex_1, NULL);
    pthread_mutex_init(&mutex_2, NULL);
    pthread_create(&tid1, NULL, child1, NULL);
    pthread_create(&tid2, NULL, child2, NULL);

    if (tid1 != 0) {
        pthread_join(tid1, NULL);
        printf("线程1已经结束\n");
    }
    if (tid2 != 0) {
        pthread_join(tid2, NULL);
        printf("线程2已经结束\n");
    }
    printf("=================thread_wait end==================\n");

    return 0;
}
