#include "raw_socket.h"

static int get_ifindex( const char *device )
{
    int sock;
    struct ifreq ifr;

    sock=socket( AF_INET,SOCK_DGRAM, 0 );
    if ( sock < 0 )
    {
        perror( "socket" );
        return ( 0 );
    }
    memset( ( char * )&ifr, 0, sizeof( ifr ) );
    strcpy( ifr.ifr_name, device );
    if ( ioctl( sock, SIOCGIFINDEX, ( char * )&ifr ) < 0 )
    {
        perror( "ioctl" );
    }
    close( sock );
    return ifr.ifr_ifindex;
}

int main()
{
    printf( "in xaui0 init\n" );
    int xsfd;
    struct sockaddr_ll sll;
    char dmac[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0xff };
    int ifindex = 0;
    
    if ( ( ifindex = get_ifindex( "xaui0" ) ) <= 0 )
    {
        return ( -1 );
    }

    if ( ( xsfd = socket( AF_PACKET, SOCK_RAW, htons( ETH_TYPE ) ) ) < 0 )
    {
        return xsfd;
    }
    
    bzero(&sll, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_halen = 6;
    memcpy(sll.sll_addr, dmac, 6);
    sll.sll_ifindex = ifindex;
    
    uint8_t *buf = malloc(100*sizeof(uint64_t));
    memset( buf, 1, 100*sizeof(uint64_t));
    if( sendto( xsfd, buf, 100*sizeof(uint64_t), 0, ( struct sockaddr* )&sll, sizeof( sll ) ) < 0 )
    {
        perror( "sendto" );
        return -1 ;
    }
    return xsfd;
}

