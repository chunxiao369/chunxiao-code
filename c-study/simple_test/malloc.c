#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define cvmx_spinlock_t            pthread_spinlock_t

#define cvmx_spinlock_init( lock )        pthread_spin_init( lock, 0)
#define cvmx_spinlock_lock       pthread_spin_lock
#define cvmx_spinlock_unlock    pthread_spin_unlock
#define cvmx_spinlock_trylock   pthread_spin_trylock

#define spinlock_t               cvmx_spinlock_t
#define spin_lock_init         cvmx_spinlock_init
#define spin_unlock            cvmx_spinlock_unlock
#define spin_lock                cvmx_spinlock_lock
#define spin_trylock            cvmx_spinlock_trylock

/* only malloc not free 
 * os is running very well
 */
#define MAX_SIZE (2 << 20)
int main()
{
    spinlock_t alock;
    char *p[1024] = {0};
    int i;
    spin_lock_init(&alock);
    printf("lock: %lu.\n", (uint64_t)alock);
    spin_lock(&alock);
    printf("lock: %lu.\n", (uint64_t)alock);
    spin_unlock(&alock);
    printf("lock: %lu.\n", (uint64_t)alock);
    printf("%d.\n",inet_addr("ip=202.255.255.255"));
    for (i = 0; i < 1024; i++) {
        usleep(10000);
        p[i] = malloc(MAX_SIZE);
        if (p[i] == NULL) {
            printf("null, return, %d\n", i);
            return 0;
        }
    }

    for (i = 0; i < 1024; i++) {
        memset(p[i], 0, MAX_SIZE);
        memcpy(p[i], &i, 100);
        usleep(10000);
        if (p[i] == NULL) {
            printf("null\n");
        } else {
            //free(p[i]);
        }
    }
    return 0;
}

