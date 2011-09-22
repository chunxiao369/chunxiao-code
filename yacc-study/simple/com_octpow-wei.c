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

typedef union  {
    uint64_t  preamble:32;
	uint64_t  type:8;
	uint64_t  paload:24;

//这里还要好好地写结构，因为通过POW0发送的包要靠这个来构建。

   /*
    uint64_t  u64;
    struct {
        uint64_t    opcode:16;  
        uint64_t    param:8;    
        uint64_t    dport:6;    
        uint64_t    rlenssz:14; 
        uint64_t    scatter:1;  
        uint64_t    rid:19;     
    }s;
    */

} cvmx_pci_inst_packet_t;


/**************************************************************

                                create and send the packet


**************************************************************/
static int32_t com_opcode_base=0x40;

static int32_t build_packet(char   *buf,uint32_t opcode)
{

		cvmx_pci_inst_packet_t *irh;
	
		memset(buf,0,16);
		buf[12] = (STAP_ETH_P & 0xFF00)>>8;
		buf[13] = (STAP_ETH_P & 0x00FF);
	
		irh = (cvmx_pci_inst_packet_t *)(buf+16);
		memset(irh,0,sizeof(cvmx_pci_inst_packet_t));
		irh->s.opcode= com_opcode_base + opcode;
	
		switch(opcode)
		{
			case PA_STAP_OPCODE_SET_CONFIG:
				irh->s.rlenssz = sizeof(pa_stap_set_config_resp_t);
				break;
	
			case PA_STAP_OPCODE_SET_UD:
				irh->s.rlenssz = sizeof(pa_stap_set_ud_resp_t);
				break;
				
			case PA_STAP_OPCODE_FIVE_TUPLE:
				irh->s.rlenssz = sizeof(pa_stap_five_tuple_resp_t);
				break;	
	
			case PA_STAP_OPCODE_UD_RULE:
				irh->s.rlenssz = sizeof(pa_stap_ud_rule_resp_t);
				break;
				
			case PA_STAP_OPCODE_SIGPROTO:
				irh->s.rlenssz = sizeof(pa_stap_sigproto_resp_t);
				break;
	
			case PA_STAP_OPCODE_SET_RULE:
				irh->s.rlenssz = sizeof(pa_stap_set_rule_resp_t);
				break;
				
			case PA_STAP_OPCODE_GET_RULE:
				irh->s.rlenssz = sizeof(pa_stap_get_rule_resp_t);
				break;
			
			case PA_STAP_OPCODE_INGRP_CHANGE:
				irh->s.rlenssz = sizeof(pa_stap_rule_ingroup_resp_t);
				break;
	
			case PA_STAP_OPCODE_OUTGRP_CHANGE:
				irh->s.rlenssz = sizeof(pa_stap_rule_outgroup_resp_t);
				break;
	
			default:
				break;
		}
	
		return 0;
}



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

    if((ifindex=get_ifindex("pow0"))<=0) {
            return(-1);
    }

    get_macaddr("pow0",dmac);

	printf("dmac = %x:%x:%x:%x:%x:%x\n",dmac[0],dmac[1],dmac[2],dmac[3],dmac[4],dmac[5]);

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



static int32_t octpow_packet_send_recv(void * buf, int32_t size, void *resp,int32_t outszie)
{
    int32_t recvlen;

	{////////////////////////////
    int32_t  i; 
    uint8_t   *p=buf; 
    for(i=0;i<size;i++)
	{
		if (i > 0 && i % 16 == 0)
			printf("\n");
        printf("%02x ",p[i]&0xff);
	}
	printf("=====size:%d ========\n",size); 
    }
       
    if(sendto(psfd_pow, buf, size , 0, (struct sockaddr*)&sll_pow, sizeof(sll_pow)) < 0) 
    {
        perror("sendto");
        return(-1);
    }

    recvlen = recvfrom(psfd_pow, resp, outszie, 0, NULL, NULL);

	{////////////////////////////
	int32_t  i; 
    uint8_t   *p=(uint8_t   *)resp; 
    for(i=0;i<recvlen;i++) 
        printf("%02x ",p[i]&0xff);
		if (i > 0 && i % 16 == 0)
			printf("\n");
    printf("]]]]]size=%d[[[[[[[\n",recvlen); 
	}   

    return recvlen;
}


static int32_t  octpow_intf_close(void)
{
    return close(psfd_pow);
}


/***********************************************************
*
*                      OCTEON  
*
************************************************************/

int32_t octpow_packet(void)
{
    pa_stap_set_config_front_t front;
    pa_stap_set_config_resp_t resp;
    
    build_packet(front.instr,PA_STAP_OPCODE_SET_CONFIG);

    front.config_mask = PA_STAP_CONFIG_MASK_INIT;
    front.config.flow_bucket = 18;
    front.config.flow_number = 12000000;
    front.config.timer_tick = 4000;
    front.config.timer_max_tick = 100;
    front.config.maintain_once_time = 10;
    front.config.flow_expire_time = 60;
	
    if(octpow_packet_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
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

int main()
{
    int32_t r_v;

//初始化POW0
	r_v = octpow_Initialize();



//然后发送一个命令或一个数据包。
    //r_v = octpow_del_rule_all(); 
    r_v = octpow_packet(); 




//关闭POW0
    r_v = octpow_Finalize();
	return 0;
}

