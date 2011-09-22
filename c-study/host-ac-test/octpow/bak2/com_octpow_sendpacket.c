/*******************************************************************/
//
//
//
/*******************************************************************/
#include "com_octpow_sendpacket.h"

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

#include "../include/api.h"
#include "../include/com_common_defs.h" 
#include "../include/com_opcode_defs.h"
#define STAP_ETH_P 0x3322

/***********************************************************
 *
 *                   OCTEON Pow interface
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

    if((ifindex=get_ifindex("eth1"))<=0) {
            return(-1);
    }

    get_macaddr("eth1",dmac);

    printf("dmac = %02x:%02x:%02x:%02x:%02x:%02x\n",dmac[0],dmac[1],dmac[2],dmac[3],dmac[4],dmac[5]);

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
    int32_t recvlen;
    {
    int32_t  i; 
    uint8_t   *p=send;
	printf("\n"); 
    for(i=0;i<size;i++)
    {
                if (i > 0 && i % 4 == 0)
                                   printf("\n");
        printf("%02x ",p[i]&0xff);
     }
        printf("\n=====size:%d ========\n",size); 
    }

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
*/
    return recvlen;
}



/***********************************************************
*
*                      OCTEON  
*
************************************************************/
//static int32_t build_packet(cvmx_eacp_packet_t  *buf)
/*
int32_t build_packet(cvmx_eacp_packet_t  *buf)
{
        int i = 0;
        memset(buf, 0, sizeof(cvmx_eacp_packet_t));
        buf->eacp_preamble = 0x22222222;
        buf->sequence = 0x22222222;
        buf->class = 0x22;
        buf->A = 0x0;
        buf->type_eacp = 0x22;
        buf->length_eacp = 0x2222;

        buf->type_msg = 0x2222;
        buf->length_msg = 0x2222;
        
        for(i=0; i< 4; i++)
            buf->array[i] = i;

        //buf.ptr_msg_content;
        return 0;
}


int32_t octpow_one_msg_start(void)
{

    cvmx_eacp_packet_t send;
    cvmx_eacp_packet_t resev;
   
    //printf("sizeof(cvmx_eacp_packet_t)=%d",sizeof(cvmx_eacp_packet_t));
    //send = (cvmx_eacp_packet_t)malloc(sizeof(cvmx_eacp_packet_t));

        //O¶?C?²¢?½¨°	bu
		build_packet(&send);

        //printf("The octpow_set_config_start end!\n");

    if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
    
}
*/

/***********************************************************
 *
 *                   Pow outside interface
 *
 ***********************************************************/

int32_t octpow_Initialize(void)
{
    int32_t rv;    
	printf("Pow interface init.\n");
    octpow_intf_init();
    return rv; 
}

int32_t octpow_Finalize(void)
{
    printf("End!\n");
    return octpow_intf_close();
}

int32_t sequence_gene()
{
	//..............
	return 0x1;
}

int32_t help()
{
	printf("send_packet  send a packet\nhelp         show all the help\n"); 
}

int32_t data_plan_reset()
{
	EACP_HDR send;
	eacp_response_t resev;

	printf("data_plan_reset\n");
	
	memset(&send, 0, sizeof(EACP_HDR));
	
	send.preamble = 0x50434145;
	send.sequence = 0x00000001;
	send.msghdr.msgType = 0x0001;
	send.msghdr.msgLen = 0x0000;	     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;    
    return 0xffffffff;
}

//int32_t ac_add(int count,uint8_t ip[4], uint8_t mac[6])
int32_t ac_add(int count,char *ip, char *mac)
{
	eacp_ac_ip_mac_t send;
	eacp_response_t resev;
	addrIpMacStru *ip_mac;
	int i = 0, j = 0, k = 0;
	
	printf("ac_add\n");
	
	memset(&send, 0, sizeof(eacp_ac_ip_mac_t));	
	memset(ip_mac, 0, (count * sizeof(addrIpMacStru)));
	
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = 0x00000002;
	send.eacp_hdr.msghdr.msgType = 0x0002;
	send.eacp_hdr.msghdr.msgType = count *10 + 1;
	send.count = count;	
	send.ip_mac = ip_mac;
	for(i = 0; i < count; i++)
	{
/*
		for(j = 0; j < 4; j++)
		ip_mac[i].ip_addr[j] = ip[j];
		for(k = 0; k < 6; k++)
		ip_mac[i].mac_addr[k] = mac[k];
		ip++;
		mac++;
*/
    strcpy(ip_mac[i].ip_addr, ip);
    strcpy(ip_mac[i].mac_addr, mac);
	}
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t ac_del()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("ac_del\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t ac_request()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("ac_request\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t port_set()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("port_set\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t port_request()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("port_request\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t mini_jumbo_set()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("mini_jumbo_set\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_add()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_add\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_del()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_del\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_del_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_del_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_request_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_request_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_request_serial()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_request_serial\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_request()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_request\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}
int32_t sta_add()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_add\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_del_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_del_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_del()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_del\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_request_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_request_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_request_serial()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_request_serial\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_request()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_request\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

//WTP的ACL
int32_t wtp_acl_add()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_acl_add\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_acl_del_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_acl_del_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_acl_del()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_acl_del\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_acl_request_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_acl_request_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_acl_request_serial()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_acl_request_serial\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cal_request_wtp()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("cal_request_wtp\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

//STA的ACL
int32_t sta_acl_add()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_acl_add\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_acl_del_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_acl_del_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_acl_del()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_acl_del\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_acl_request_all()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_acl_request_all\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_acl_request_serial()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("sta_acl_request_serial\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cal_request_sta()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("cal_request_sta\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cpu_set()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("cpu_set\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t data_plan_run_time()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_run_time\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t data_plan_cpu()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_cpu\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t data_plan_packet()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_packet\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cpu_high_alert()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("cpu_high_alert\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cpu_high_clear()
{
	EACP_HDR send;
	eacp_response_t resev;
	printf("cpu_high_clear\n");
	
	//...
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}
