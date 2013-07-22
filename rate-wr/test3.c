#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

#define DATA_SIZE  (1024*1024*1024)
#define DATA_LEN   (1024)
#define DATA_TIMES (1024*1024)

int main()
{
    int fd;
    void *src;
    char *p;
    int i, j;
    time_t t_b;
    time_t t_e;

    t_b = time(NULL);
    fd = open("data.in", O_RDWR);
    if ((src = mmap(0, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        printf("src map error/n");
        return -1;
    }
#if 1
    p = (char *)src;
    for (i = 0; i < DATA_TIMES; i++) {
        //for (j = 0; j < DATA_LEN; j++) {
            //p[j]++;
        for (j = 0; j < DATA_LEN; j += sizeof(int)) {
            int *p_i = (int *)p;
            p_i[0]++;
        }
        p += DATA_LEN;
    }
#endif
    t_e = time(NULL);
    printf("tb: %lu, te: %lu.\n", t_b, t_e);
    printf("ok! seconds: %lu, rate: %luMbps.\n", t_e - t_b, (DATA_SIZE / (t_e - t_b)) >> 17);
    close(fd);
    return 0;
}
