
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include "b.h"

#define MAX 1

pthread_t thread[2];
pthread_mutex_t mut;
int number = 0;

extern int a(void);
int(*p_myprint)(void);
int(*p_myinc)(void);
void *thread1()
{
    int i;
    printf("thread1 : I'm thread 1\n");

    for (i = 0; i < MAX; i++) {
        printf("thread1 : number = %d\n", number);
        a();
        b_inc();
        b_print();

        void *phandle = dlopen("liba.so", RTLD_LAZY);
        p_myinc = dlsym(phandle, "b_inc");
        p_myinc();
        p_myprint = dlsym(phandle, "b_print");
        p_myprint();

#if 0
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(1);
#endif
    }

    printf("thread1 :waiting for me？\n");
    pthread_exit(NULL);
}

void *thread2()
{
    int i;
    printf("thread2 : I'm thread 2\n");

    for (i = 0; i < MAX; i++) {
        printf("thread2 : number = %d\n", number);
        a();
#if 0
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
#endif
    }

    printf("thread2 : waiting for me?\n");
    pthread_exit(NULL);
}

void thread_create(void)
{
    int temp;
    memset(&thread, 0, sizeof(thread)); //comment1
    /*创建线程 */
    if ((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)  //comment2
        printf("thread 1 creat failed!\n");
    else
        printf("thread 1 create success\n");

#if 0
    if ((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)  //comment3
        printf("thread 1 creat failed!\n");
    else
        printf("thread 2 create success\n");
#endif
}

void thread_wait(void)
{
    printf("==================thread_wait start=================\n");
    /*等待线程结束 */
    if (thread[0] != 0) {       //comment4
        pthread_join(thread[0], NULL);
        printf("thread 1 success\n");
    }
#if 0
    if (thread[1] != 0) {       //comment5
        pthread_join(thread[1], NULL);
        printf("thread 2 success\n");
    }
#endif
    printf("=================thread_wait end==================\n");
}

int main(int argc, char *argv[])
{
    /*用默认属性初始化互斥锁 */
    pthread_mutex_init(&mut, NULL);

    thread_create();
    thread_wait();
    printf("main thread sucess!\n");

    return 0;
}
