#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define size 1024*1024*500
#define LEN 1024
int main()
{
    int fdin, fdout;
    struct stat statbuf;
    void *src, *dst;
    char *p;
    int i, j;
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

    if ((src = mmap(0, size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
        printf("src map error/n");
        return -1;
    }
    lseek(fdout, size - 1, SEEK_SET);
    write(fdout, "\0", 1);
    //因为data.out是空文件，必须创建一个空洞让其大小成为size来进行下面的mmap

    if ((dst = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED) {
        printf("dest map error/n");
        return -1;
    }
    memcpy(dst, src, size);
    p = (char *)dst;
    for (i = 0; i < size / LEN; i++) {
        for (j = 0; j < LEN; j++)
            p[j]++;
        p += LEN;
    }
    printf("ok/n");
    close(fdout);
    return 0;
}
