
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include "dlist.h"
#include "locker_pthread.h"

static void *producer(void *param)
{
    int i = 0;
    list_t *dlist = (list_t *) param;

    for (i = 0; i < 100; i++) {
        assert(list_add(dlist, (void *)i) == RET_OK);
    }
    sleep(1);
    for (i = 0; i < 100; i++) {
        assert(list_add(dlist, (void *)i) == RET_OK);
    }

    return NULL;
}

static void *consumer(void *param)
{
    int i = 0;
    list_t *dlist = (list_t *) param;

    for (i = 0; i < 2; i++) {
        usleep(20);
        printf("length: %d.\n", list_length(dlist));
        //assert(list_delete(dlist, 0) == RET_OK);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t consumer_tid = 0;
    pthread_t producer_tid = 0;
    locker_t *locker = locker_loop_create(
                       locket_pthread_self,
                       locker_pthread_lock,
                       locker_pthread_unlock,
                       locker_pthread_destroy,
                       locker_pthread_init,
                       sizeof(PrivInfo));
    list_t *dlist = list_create(NULL, NULL, locker);
    pthread_create(&producer_tid, NULL, producer, dlist);
    pthread_create(&consumer_tid, NULL, consumer, dlist);
    pthread_join(consumer_tid, NULL);
    pthread_join(producer_tid, NULL);

    printf("length: %d.\n", list_length(dlist));
    list_destroy(dlist);
    return 0;
}
