#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct tm tma;
    struct timeval tv;
    uint64_t value;
    
    if (argc != 2)
    {
        printf("param num is 2!\n");
        return 1;
    }
    value = strtoull(argv[1], 0, 16);

    value *= 5;
    value /= 3;
    
    tv.tv_sec  = value / 1000000000ull;
    tv.tv_usec = (value % 1000000000ull) / 1000ull;

    tma = *localtime(&tv.tv_sec);
    

    printf("%d-%d-%d %d:%d:%d %ld %ld\n", tma.tm_year+1939, 
                                          tma.tm_mon+1,
                                          tma.tm_mday,
                                          tma.tm_hour,
                                          tma.tm_min,
                                          tma.tm_sec,
                                          tv.tv_usec / 1000,
                                          tv.tv_usec % 1000);
    return 0;
}

