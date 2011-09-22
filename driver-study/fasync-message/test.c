
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define KPP_GET     _IOR('f', 2, int)
static int fd;

void gpiovol_handler(int sigum) 
{
    int value;
    ioctl(fd, KPP_GET, &value);
    printf("the value = %d.\n", value);
} 

int main() 
{
    int oflags;
    fd = open("/dev/hold", O_RDWR);
    if (fd < 0) {
        return -1;
    }
    // 当按下hold键后，hold driver将向该进程发送SIGIO信号
    signal(SIGIO, gpiovol_handler);

    // F_SETOWN设置接收SIGIO和SIGURG信号的进程ID或进程组ID.正的
    // arg指定一个进程ID，负的arg表示等于arg绝对值的一个进程组ID
    fcntl(fd, F_SETOWN, getpid());
    oflags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, oflags | FASYNC); // (1)
    while (1) ;
    return 0;
}

