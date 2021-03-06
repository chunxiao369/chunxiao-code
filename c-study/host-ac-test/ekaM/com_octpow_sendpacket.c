/** 
 * @file    com_bcm.c
 * @brief   octeon communiction. 
                Function octpow_del_rule_all is used by 'del rule all' command.
                Function octpow_del_rule is used by 'del rule rule_id entry' command.
                Function octpow_del_rule_param is used by 'del rule rule_id' command.
 * @author  xu.chunxiao@embedway.com
 * @version  0.1 
 * @date   2009.11.11
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <net/if.h>

#include "api.h"
#include "com_common_defs.h" 
#include "com_opcode_defs.h"
#define STAP_ETH_P 0x3322
#define  bcm_debug_print( format, args... )    //printf( "  DEBUG Message:"format, ##args )

typedef struct  {
    uint64_t  eacp_preamble:32;	
	uint64_t  sequence:32;	
	uint64_t  class:8;
	uint64_t  A:1;
	uint64_t  type_eacp:7;
	uint64_t  length_eacp:16;	

	uint64_t  type_msg:16;
	uint64_t  length_msg:16;

	// point to the msg's content
	//uint32_t  ptr_msg_content;
	
} cvmx_eacp_packet_t;

/***********************************************************
 *
 *                                  OCTEON Pow interface
 *
 ***********************************************************/
int32_t get_ifindex(const char   *device)
{
   int32_t sock;
   struct ifreq ifr;

   sock=socket(AF_INET,SOCK_DGRAM, 0);
   if(sock<0) 
   { 
        perror("socket"); 
       return(0); 
   }
   memset((char   *)&ifr, 0, sizeof(ifr));
   strcpy(ifr.ifr_name, device);
   if (ioctl(sock, SIOCGIFINDEX, (char   *)&ifr) < 0) 
   {
        perror("ioctl");
   }
    close(sock);
								    
    return ifr.ifr_ifindex;
}

int32_t get_macaddr(char   *device,char   *mac)
{
    int32_t sock;
    struct ifreq ifr;

    strncpy(ifr.ifr_name,device,IFNAMSIZ);
    memset(mac,0x00,6);
    sock=socket(AF_INET,SOCK_DGRAM, 0);
    if(sock<0) 
   { 
   //    perror("socket\n"); 
            return -1; 
    }
    if ( ioctl( sock, SIOCGIFHWADDR, (caddr_t)&ifr) < 0) 
    {
         close(sock);
        return -1;
    }
    memcpy(mac,ifr.ifr_hwaddr.sa_data, 6);
    close(sock);
    return 0;
}


int32_t psfd_pow;
struct sockaddr_ll sll_pow;

static int32_t  octpow_intf_init(void)
{
    char   dmac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int32_t ifindex=0;

    if((ifindex=get_ifindex("eth0"))<=0) {
            return(-1);
    }

    get_macaddr("eth0",dmac);

	//printf("dmac = %x:%x:%x:%x:%x:%x\n",dmac[0],dmac[1],dmac[2],dmac[3],dmac[4],dmac[5]);

    if ((psfd_pow = socket(AF_PACKET, SOCK_RAW, htons(STAP_ETH_P))) < 0){
            return psfd_pow;;
    }

    bzero(&sll_pow, sizeof(sll_pow));
    sll_pow.sll_family = AF_PACKET;
    sll_pow.sll_halen = 6;
    memcpy(sll_pow.sll_addr, dmac, 6);
    sll_pow.sll_ifindex = ifindex;

    return psfd_pow;

}

static int32_t  octpow_intf_close(void)
{
    return close(psfd_pow);
}

static int32_t octpow_cmd_send_recv(void * send, int32_t size, void *resev,int32_t outszie)
{
    //int32_t recvlen;
    {
    int32_t  i; 
    uint8_t   *p=send; 
    for(i=0;i<size;i++)
	{
		if (i > 0 && i % 4 == 0)
				   printf("\n");
        printf("%02x ",p[i]&0xff);
	}
	printf("\n=====size:%d ========\n",size); 
    }
	
	//build_packet();    
   
    if(sendto(psfd_pow, send, size , 0, (struct sockaddr*)&sll_pow, sizeof(sll_pow)) < 0) 
    {
        perror("sendto");
        return(-1);
    }

/*
    recvlen = recvfrom(psfd_pow, resev, outszie, 0, NULL, NULL);

	{
	int32_t  i; 
    uint8_t   *p=(uint8_t   *)resev; 
    for(i=0;i<recvlen;i++) 
        printf("%02x",p[i]&0xff);
    printf("]]]]]size=%d[[[[[[[\n",recvlen); 
	}   
    return recvlen;
*/
	return 1
}



/***********************************************************
*
*                      OCTEON  
*
************************************************************/
static int32_t build_packet(cvmx_eacp_packet_t  *buf)
{
	memset(buf, 0, sizeof(cvmx_eacp_packet_t));
	buf->eacp_preamble = 0x22222222;	
	buf->sequence = 0x22222222;	
	buf->class = 0x22;
	buf->A = 0x0;
	buf->type_eacp = 0x22;
	buf->length_eacp = 0x2222;	

	buf->type_msg = 0x2222;
	buf->length_msg = 0x2222;
	
	//buf.ptr_msg_content;
	return 0;
}


int32_t octpow_one_msg_start(void)
{

    cvmx_eacp_packet_t send;
    cvmx_eacp_packet_t resev;

	//判断待发送的数据包类型并组建包。
	build_packet(&send);

	//printf("The octpow_set_config_start end!\n");

    if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
    
}


int32_t octpow_Initialize(void)
{
    int32_t rv;    
    octpow_intf_init();
    return rv; 
}

int32_t octpow_Finalize(void)
{
    return octpow_intf_close();
}

/*
int main()
{
    int32_t r_v;

//初始化POW0
	r_v = octpow_Initialize();


//然后发送一个命令
    r_v = octpow_one_msg_start(); 

//关闭POW0
    r_v = octpow_Finalize();
	return 0;
}
*/
