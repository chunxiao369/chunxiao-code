
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

int main() 
{
    int oflags;
    int value;
    fd = open("/dev/kpp", O_RDWR);
    if (fd < 0) {
        printf("open error!\n");
        return -1;
    }
    ioctl(fd, KPP_GET, &value);
    printf("the value = %d.\n", value);

    return 0;
}

