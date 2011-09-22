
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>

const char *dev_file_name = "/dev/failover";

void input_handler(int num)
{
    printf("notified\n");
}

int main (int argc, char* argv[])
{
    int enable;
    int fd;
    int oflags;
    
    /* open file */
    fd = open(dev_file_name, O_RDWR, 0);
    if (fd < 0) {
        printf("Error while open file!\n");
        return 1;
    }

    signal(SIGIO, input_handler);
    fcntl(fd, F_SETOWN, getpid());
    oflags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, oflags | FASYNC);

    while(1);

    /* close file */
    if (fd) {
        close(fd);
    }

    return 0;
}

