#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define DATA_SIZE  (1024ull*1024ull*1024ull)
#define DATA_LEN   (1 << 20)
#define DATA_TIMES (DATA_SIZE / DATA_LEN)

int main()
{
    int fdin, fdout;
    void *src, *dst;
    char *p;
    int i, j;
    int ret;
    uint64_t offset = 0;

    time_t t_b;
    time_t t_e;

    fdin = open("data.in", O_RDONLY);
    if (fdin == -1) {
        printf("open data.in error!\n");
        return -1;
    }
    fdout = open("data.out", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if (fdout == -1) {
        printf("open data.out error!\n");
        return -1;
    }
    //因为data.out是空文件，必须创建一个空洞让其大小成为DATA_SIZE来进行下面的mmap
    lseek(fdout, DATA_SIZE - 1, SEEK_SET);
    ret = write(fdout, "\0", 1);
    if (ret == -1) {
        printf("write file error!\n");
        return -1;
    }
    printf("page size: %ld.\n", sysconf(_SC_PAGE_SIZE));

    t_b = time(NULL);
    offset = 0ull;
    for (i = 0; i < DATA_TIMES; i++) {
        if ((src = mmap(0, DATA_LEN, PROT_READ, MAP_SHARED, fdin, offset)) == MAP_FAILED) {
            printf("src map error\n");
            printf("i is %d.\n", i);
            return -1;
        }
        if ((dst = mmap(0, DATA_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, offset)) == MAP_FAILED) {
            printf("dest map error\n");
            printf("i is %d.\n", i);
            return -1;
        }
        memcpy(dst, src, DATA_LEN);
#if 0
        munmap(src, DATA_LEN);
        munmap(dst, DATA_LEN);
#endif
        offset += DATA_LEN;
    }
#if 0
    p = (char *)dst;
    for (i = 0; i < DATA_SIZE / DATA_LEN; i++) {
        for (j = 0; j < DATA_LEN; j++)
            p[j]++;
        p += DATA_LEN;
    }
#endif
    t_e = time(NULL);
    printf("ok! seconds: %lu, rate: %lluMbps.\n", t_e - t_b, (DATA_SIZE / (t_e - t_b)) >> 17);
    close(fdout);
    close(fdin);
    return 0;
}
