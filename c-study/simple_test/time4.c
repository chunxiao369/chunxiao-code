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
    //uint64_t value;
    uint64_t timestamp;
    time_t current;
    struct timeval cur;

	int a = 'c';
	printf("aaaaaa %d.\n", a);
	printf("aaaaaa %d.\n", (int)'类');
	printf("aaaaaa %d.\n", '类');
	printf("aaaaaa %s.\n", (char *)15184315);
	return 0;

    gettimeofday( &cur, NULL );
    //timestamp = cur.tv_sec * 1000000000ull + cur.tv_usec * 1000ull - ( 365 * 39 + 10 ) * 86400ull * 1000000000ull; 
    timestamp = cur.tv_sec * 1000000000ull + cur.tv_usec * 1000ull; 
    
    //value = timestamp;
    printf("timestamp: %llu\n", timestamp);
    tv.tv_sec  = timestamp / 1000000000ull;
    tv.tv_usec = (timestamp % 1000000000ull) / 1000ull;

    tma = *localtime(&tv.tv_sec);
    printf("%d-%d-%d %d:%d:%d %ld %ld\n", tma.tm_year, 
                                          tma.tm_mon+1,
                                          tma.tm_mday,
                                          tma.tm_hour,
                                          tma.tm_min,
                                          tma.tm_sec,
                                          tv.tv_usec / 1000,
                                          tv.tv_usec % 1000);
    
    printf("time string: %s.\n", asctime(&tma));
    return 0;
}

