
#include <unistd.h>
#include "log4c.h"
#include "pthread.h"

log4c_category_t* mycat;
static void *thread_func(void *arg)
{
    int i = 0;

    while (1) {
        usleep(1);

        log4c_category_log(mycat, LOG4C_PRIORITY_CRIT, "Hellocrit!, %d", i);
        i++;
        if (i >= 1000)
            break;
    }
    return NULL;
}

int main()
{
    int i = 0;
    pthread_t tid;

    if (log4c_init()) {
        printf("aaaaaaaaaaa");
        return 0;
    }
    mycat = log4c_category_get("octeon_cat");
    printf("mycat %p.\n", mycat);
    //pthread_create(&tid, NULL, thread_func, NULL);

    while (1) {
        usleep(1);
        log4c_category_log(mycat, LOG4C_PRIORITY_CRIT, "crit!, %d", i);
        log4c_category_log(mycat, LOG4C_PRIORITY_ERROR, "error!, %d", i);
        log4c_category_log(mycat, LOG4C_PRIORITY_WARN, "warn!, %d", i);
        log4c_category_log(mycat, LOG4C_PRIORITY_INFO, "info!, %d", i);
        log4c_category_log(mycat, LOG4C_PRIORITY_DEBUG, "debug!, %d", i);
        log4c_category_log(mycat, LOG4C_PRIORITY_TRACE, "trace!, %d", i);
        i++;
        if (i >= 1)
            break;
    }
    //pthread_join(tid, NULL);
    return 0;
}

