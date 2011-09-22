
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
#include <arpa/inet.h>
unsigned short checksum(unsigned short *buf, int nword)
{
    unsigned long sum;

    for(sum = 0; nword > 0; nword--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return ~sum;
}

uint16_t ip_checksum(uint16_t*ip_hdr)
{
    int i=0;
    uint32_t checksum = *ip_hdr;
  
    for ( i = 1; i < 10; i++ ) 
    {
        checksum +=  *(ip_hdr + i);   
        checksum = ( checksum & 0xffff ) + ( checksum >> 16 );  
    }

    return 0xffff - checksum;   
}

int main()
{
    uint16_t a;
    uint64_t timestamp;

    struct timeval tv;
    unsigned char buf[20] = 
    {0x45, 0x00, 0x00, 0x40, 0xab, 0x54, 0x00, 0x00, 0x7f, 0x11,
     0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0xc0, 0xa8, 0x00, 0x67};
    
    a = ip_checksum((uint16_t *)buf);
    printf("%04x\n", a);
    a = checksum((uint16_t *) buf, 10);
    printf("%04x\n", a);
    gettimeofday( &tv, NULL );
    timestamp = tv.tv_sec * 1000000000ull + tv.tv_usec * 1000ull - ( 365 * 39 + 10 ) * 86400ull * 1000000000ull;
    //temp = timestamp * 600 /1000;
    printf("%llu\n", timestamp);
    return 0;
}

