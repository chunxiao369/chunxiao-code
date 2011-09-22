#include <limits.h>
#include <pthread.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *thread_routine(void *arg)
{
    int *p;
    p = (int *)arg;

    printf ("The thread is here %d\n",*p );
    while (*p < 100)
    {
        printf ("The thread is here %d\n",*p );
        continue; 
    }
    printf("thread routine finish!\n");
    *p = 1000;

    return NULL;
}

int main (int argc, char *argv[])
{
    pthread_t thread_id;
    pthread_attr_t thread_attr;
    int status;
    int i = 4;

    status = pthread_attr_init (&thread_attr);
    if (status != 0)
        printf("Create attr");

    status = pthread_attr_setdetachstate (
        &thread_attr, PTHREAD_CREATE_DETACHED);

    if (status != 0)
        printf("Set detach");
    status = pthread_create (&thread_id, &thread_attr, thread_routine, (void *)&i);
    if (status != 0)
        printf("error\n"); 
    sleep(1); 
    for (i = 4; i <= 100; i++)
    {
        printf("time: %d, address: %p.\n", i, &i);
    }
    printf ("Main exiting\n");
    pthread_exit (NULL);
    return 0;
}

