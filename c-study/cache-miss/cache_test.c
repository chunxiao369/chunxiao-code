#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#define CACHE_ALIGNMENT 128

#define PERF_WELL
#ifdef PERF_WELL
#define attr_cache_aligned __attribute__((aligned(CACHE_ALIGNMENT * 2)))
#else
#define attr_cache_aligned
#endif

typedef struct _cnt {
    uint32_t counter;
} attr_cache_aligned cnt_t;

cnt_t cnt[32];

void *myfun(void *arg)
{
    char buf[256];
    int coreid;
    uint64_t i = 0;
    coreid = sched_getcpu();
    printf("thread coreid: %d.\n", coreid);
    while (i < (1ul << 30)) {
        memset(buf, i, sizeof(buf));
        cnt[coreid].counter = (uint32_t)i;
        i++;
    }
    pthread_exit(NULL);
}

int func_c(uint64_t core_mask)
{
    int i;
    cpu_set_t mask;
    pthread_t tid[64];
	pthread_attr_t attr;
    int num = sysconf(_SC_NPROCESSORS_CONF);

    printf("system has %d processor(s), use 0x%lx\n", num, core_mask);
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
		pthread_attr_init(&attr);
        CPU_ZERO(&mask);
        CPU_SET(i, &mask);
		pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &mask);
        if (pthread_create(&tid[i], &attr, (void *)myfun, NULL) != 0) {
            fprintf(stderr, "thread create failed\n");
            return -1;
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
#define ARRAY_SIZE 8192 

int func_a(int num)
{
    int i;
    int j;
    uint8_t *a = NULL;

    a = (uint8_t *)malloc(ARRAY_SIZE * ARRAY_SIZE);
    if (NULL == a) {
        printf("malloc failed.\n");
        return 0;
    }
    if (1 == num) {
        for (i = 0; i < ARRAY_SIZE; ++i) {
            for (j = 0; j < ARRAY_SIZE; ++j) {
                a[i * ARRAY_SIZE + j] = 2;
            }
        }
    } else {
        for (i = 0; i < ARRAY_SIZE; ++i) {
            for (j = 0; j < ARRAY_SIZE; ++j) {
                a[j * ARRAY_SIZE + i] = 2;
            }
        }
    }
    free(a);
    return 0;
}

int func_b(void)
{
    int i;
    i = atoi("333");
    i = i + i;
    return i;
}

int main(int argc, char **argv)
{
    func_a(argc);
    func_b();
    func_c(0xful);
    return 0;
}

