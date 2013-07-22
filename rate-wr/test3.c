#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#define size 1024*1024*500
#define LEN 1024
int main()
{
    int fd;
    void *src;
    char *p;
    int i, j;
    fd = open("data.in", O_RDWR);
    if ((src = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        printf("src map error/n");
        return -1;
    }
    p = (char *)src;
    for (i = 0; i < size / LEN; i++) {
        for (j = 0; j < LEN; j++)
            p[j]++;
        p += LEN;
    }
    printf("ok/n");
    close(fd);
}
