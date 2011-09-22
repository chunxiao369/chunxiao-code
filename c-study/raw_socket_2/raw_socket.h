
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <linux/sockios.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <fcntl.h>
#include <linux/if_packet.h>

#define DESTPORT        80       /* ......(WEB)      */ 
#define LOCALPORT       8888 
#define IFNAME          "xaui0"
#define ETH_TYPE        0x9988
#define NAME_LENGTH     80
#define FRU_LENGTH      16

void send_tcp(int sockfd,struct sockaddr_in *addr); 
unsigned short check_sum(unsigned short *addr,int len); 

typedef struct probe_s{
    uint32_t device_id;
    uint8_t  name[NAME_LENGTH];
    uint32_t mgmt_ip;
    uint8_t  fru_info[FRU_LENGTH];
    uint8_t  port;
    
}probe_t;

