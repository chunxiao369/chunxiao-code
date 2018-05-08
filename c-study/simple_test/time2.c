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
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>

int settime(int year, int month, int day,
            int hour, int minute, int second)
{
    struct tm tma;
    struct timeval tim_a;
    struct timezone zone_a;

    tma.tm_sec      = second;
    tma.tm_min      = minute;
    tma.tm_hour     = hour;
    tma.tm_mday     = day;
    tma.tm_mon      = month - 1;
    tma.tm_year     = year - 1900;
    //asctime(&tma);
    memset(&tim_a, 0, sizeof(tim_a));
    memset(&zone_a, 0, sizeof(zone_a));
    tim_a.tv_sec = mktime(&tma);
    if (settimeofday(&tim_a, &zone_a)) {
        printf("sssssss.\n");
        return 1;
    }
    return 0;
}

int show_time()
{
    struct timeval cur;
    struct tm tma;
    gettimeofday( &cur, NULL );
    //printf("time string: %s.\n", ctime(&cur.tv_sec));
    tma = *localtime(&cur.tv_sec);
    printf("%ld\n", cur.tv_sec);
    printf("%d-%d-%d %d:%d:%d\n", tma.tm_year + 1900,
                                  tma.tm_mon + 1,
                                  tma.tm_mday,
                                  tma.tm_hour,
                                  tma.tm_min,
                                  tma.tm_sec);
    return 0;
}

int main(int argc, char *argv[])
{
    //settime(2012, 12, 12, 12, 12, 12);
    show_time();
#if 0
    struct tm tma;
    struct timeval tv;
    //uint64_t value;
    uint64_t timestamp;
    time_t current;
    struct timeval cur;

    gettimeofday( &cur, NULL );
    timestamp = cur.tv_sec * 1000000000ull + cur.tv_usec * 1000ull - ( 365 * 39 + 10 ) * 86400ull * 1000000000ull;

    //value = timestamp;
    printf("timestamp: %llu\n", timestamp);
    tv.tv_sec  = timestamp / 1000000000ull;
    tv.tv_usec = (timestamp % 1000000000ull) / 1000ull;

    printf("time string: %s.\n", ctime(&cur.tv_sec));
    tma = *localtime(&tv.tv_sec);
    printf("%d-%d-%d %d:%d:%d %ld %ld\n", tma.tm_year+1939,
                                          tma.tm_mon+1,
                                          tma.tm_mday,
                                          tma.tm_hour,
                                          tma.tm_min,
                                          tma.tm_sec,
                                          tv.tv_usec / 1000,
                                          tv.tv_usec % 1000);
#endif
#if 0
    timestamp = timestamp + ( 365 * 39 + 10 ) * 86400ull * 1000000000ull;
    tv.tv_sec  = timestamp / 1000000000ull;
    tv.tv_usec = (timestamp % 1000000000ull) / 1000ull;
    tma = *localtime(&tv.tv_sec);
    printf("aaaaaaaaaaa%d-%d-%d %d:%d:%d %ld %ld\n", tma.tm_year + 1900,
                                          tma.tm_mon+1,
                                          tma.tm_mday,
                                          tma.tm_hour,
                                          tma.tm_min,
                                          tma.tm_sec,
                                          tv.tv_usec / 1000,
                                          tv.tv_usec % 1000);
    tma.tm_mon = 25;
    current = mktime(&tma);

    tma = *gmtime(&current);
    printf("%d-%d-%d %d:%d:%d %ld %ld\n", tma.tm_year+1939,
                                          tma.tm_mon+1,
                                          tma.tm_mday,
                                          tma.tm_hour,
                                          tma.tm_min,
                                          tma.tm_sec,
                                          tv.tv_usec / 1000,
                                          tv.tv_usec % 1000);

    printf("time string: %s.\n", ctime(&current));
    printf("time string: %s.\n", asctime(&tma));
#endif
    return 0;
}

