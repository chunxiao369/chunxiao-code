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
//use to srand()
#include <time.h>

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

    if((ifindex=get_ifindex("eth0"))<=0) {
            return(-1);
    }

    get_macaddr("eth0",dmac);

    printf("eth0.dmac = %02x:%02x:%02x:%02x:%02x:%02x\n",dmac[0],dmac[1],dmac[2],dmac[3],dmac[4],dmac[5]);

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

/////////////////////////////////////////////////////////////////////////////////////////
//                                             common function
/////////////////////////////////////////////////////////////////////////////////////////
int32_t sequence_gene()
{
	//..............
    srand((unsigned)time(NULL));	
	return rand();
}



/////////////////////////////////////////////////////////////////////////////////////////
//                                                Command function
/////////////////////////////////////////////////////////////////////////////////////////

int32_t help()
{
	printf("send_packet  send a packet\nhelp         show all the help\n"); 
}

int32_t data_plan_reset()
{
	//0x0001
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_reset\n");	
	//send = (EACP_HDR)malloc(sizeof(EACP_HDR));
	//memset(&send, 0, sizeof(EACP_HDR));	
	
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0100;
	send.msghdr.msgLen = 0x0000;	     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;    
    return 0xffffffff;
}

int32_t ac_add(int count,unsigned int ip, unsigned char *mac)
{
	//0x0002
	eacp_ac_ip_mac_t send;
	eacp_response_t resev;
	addrIpMacStru ip_mac;
	int i = 0;	
	//printf("ac_add, ip: 0x%08x.\n", ip);
    //printf("\n");
	
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0200;
	send.eacp_hdr.msghdr.msgLen= count * sizeof(addrIpMacStru) + 1;
	send.count = count;	
	send.ip_mac = &ip_mac;

	ip_mac.ip_addr = ip;
	
    for (i = 0; i < 6; i++)
    {
        //printf("mac: %02x \n", mac[i]);
    	ip_mac.mac_addr[i] = mac[i];
	}
	
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t ac_add2(char *filename)
{
	//0x0002
	eacp_ac_ip_mac_t send;
	eacp_response_t resev;		
	FILE *fin = NULL;
	int8_t str[1024] = {0}; 
	char *tmp = NULL;
	int len = 0,line = 0,i = 0;
	printf("ac_add2\n");	
    send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0200;
	//send.eacp_hdr.msghdr.msgLen = 0x0;  //...
	//send.count = 0x0;  //...
	
	fin = fopen(filename, "r");
	if(fin == NULL)
	{
	    printf("Open the input file error!\n");
		return 0x0;
	}
	fgets(str, 1024, fin);
	len = strlen(str) - 1;
	str[len] = 0;//The first line of the file ,it means the length of the file
	for(i = 0; i < len; i++)
	{
	    line = line * 10 + (str[i] - 48);
	}
	printf("The file is %d\n",line);
	
	send.count = line;
	send.eacp_hdr.msghdr.msgLen = line * sizeof(addrIpMacStru);
	send.ip_mac = (addrIpMacStru *)malloc(line * sizeof(addrIpMacStru));
	
	
	while(!feof(fin))
	{
	    fgets(str, 1024, fin);
	    str[strlen(str)-1] = 0;		
	    /*{
	      tmp = strtok( str, " " );
		send.ip_mac[i].ip_addr = inet_ntoa(tmp); 
		
		tmp = strtok( NULL, " " );
		send.ip_mac[i].mac[0] = ;
		send.ip_mac[i].mac[1] = ;
		send.ip_mac[i].mac[2] = ;
		send.ip_mac[i].mac[3] = ;
		send.ip_mac[i].mac[4] = ;
		send.ip_mac[i].mac[5] = ;
		
		}
		*/
		i++;
	}
	
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}


int32_t ac_del()
{
	//0x0003
	eacp_ac_ip_mac_t send;
	eacp_response_t resev;
	printf("ac_del\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0300;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.ip_mac = (addrIpMacStru *)malloc(sizeof(addrIpMacStru)*send.count);	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t ac_del2(char *fin)
{
	//0x0003
	eacp_ac_ip_mac_t send;
	eacp_response_t resev;
	printf("ac_del2\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0300;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.ip_mac = (addrIpMacStru *)malloc(sizeof(addrIpMacStru)*send.count);	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t ac_request()
{
	//0x0004
	EACP_HDR send;
	eacp_ac_ip_mac_t resev;
	printf("ac_request\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0400;
	send.msghdr.msgLen = 0x0;	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t port_set(int ctr_port, int data_port)
{
	//0x0005 
	eacp_port_t send;
	eacp_response_t resev;
	printf("port_set\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0500;
	send.eacp_hdr.msghdr.msgLen = 0x0400;  ////////////
	send.control_port = ctr_port;
	send.data_port = data_port;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t port_request()
{
	//0x0006
	EACP_HDR send;
	eacp_port_t resev;
	printf("port_request\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0600;
	send.msghdr.msgLen = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t mini_jumbo_set(int flag)
{
	//0x0007
	eacp_mini_jumbo_t send;
	eacp_response_t resev;
	printf("mini_jumbo_set\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0700;
	send.eacp_hdr.msghdr.msgLen = 0x0100;
	send.mini_jumbo = flag;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_add()
{
	//0x0008
	eacp_wtp_info_t send;
	eacp_wtp_response_t resev;
	printf("wtp_add\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0800;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.wtpinfo = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_del()
{
	//0x0009
	eacp_count_mac_t send;
	eacp_wtp_response_t resev;
	printf("wtp_del\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0900;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.mac = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_del_all()
{
	//0x000a 
	EACP_HDR send;
	eacp_response_t resev;
	printf("wtp_del_all\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0a00;
	send.msghdr.msgLen = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_request_all()
{
	//0x000b
	EACP_HDR send;
	eacp_wtp_info_t resev;
	printf("wtp_request_all\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0b00;
	send.msghdr.msgLen = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_request_serial()
{
	//0x000c
	eacp_start_end_t send;
	eacp_wtp_info_t resev;
	printf("wtp_request_serial\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0c00;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.start = 0x0;
	send.end = 0x0;
	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t wtp_request()
{
	//0x000d
	eacp_count_mac_t send;
	eacp_wtp_info_result_t resev;
	printf("wtp_request\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0d00;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.mac = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}
int32_t sta_add()
{
	//0x000e
	eacp_sta_info_t send;
	eacp_count_mac_t resev;
	printf("sta_add\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0e00;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.sta_info = 0x0;
	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_del_all()
{
	//0x000f
	EACP_HDR  send;
	eacp_response_t resev;
	printf("sta_del_all\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0f00;
	send.msghdr.msgLen = 0x0;
	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_del()
{
	//0x0010
	eacp_count_mac_t send;
	eacp_sta_response_t resev;
	printf("sta_del\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x1000;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.mac = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_request_all()
{
	//0x0011
	EACP_HDR send;
	eacp_sta_info_t resev;
	printf("sta_request_all\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x1100;
	send.msghdr.msgLen = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_request_serial()
{
	//0x0012
	eacp_start_end_t send;
	eacp_sta_info_t resev;
	printf("sta_request_serial\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x1200;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.start = 0x0;
	send.end = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t sta_request()
{
	//0x0013
	eacp_count_mac_t send;
	eacp_sta_info_result_t resev;
	printf("sta_request\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x1300;
	send.eacp_hdr.msghdr.msgLen = 0x0;
	send.count = 0x0;
	send.mac = 0x0;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

int32_t cpu_set(long cpu)
{
    //0x0401
	eacp_cpu_t send;
	eacp_response_t resev;
	printf("cpu_set\n");
	
	//...	
    send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0104;
	send.eacp_hdr.msghdr.msgLen = 0x0400;
	send.cpu = cpu;
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<      >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

int32_t data_plan_run_time_response()
{
    //0x2001
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_run_time\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0120;
	send.msghdr.msgLen = 0x0;
	     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t data_plan_cpu()
{
    //0x2002
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_cpu\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0220;
	send.msghdr.msgLen = 0x0;
	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t data_plan_packet()
{
    //0x2003
	EACP_HDR send;
	eacp_response_t resev;
	printf("data_plan_packet\n");
	
	//...
	send.preamble = 0x50434145;
	send.sequence = sequence_gene();
	send.msghdr.msgType = 0x0320;
	send.msghdr.msgLen = 0x0;	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cpu_high_alert(int32_t cpu)
{
    //0x4001
	eacp_cpu_t send;
	eacp_response_t resev;
	printf("cpu_high_alert\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0140;
	send.eacp_hdr.msghdr.msgLen = 0x0400;
	send.cpu = cpu;	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}

int32_t cpu_high_clear(int32_t cpu)
{
    //0x4002
	eacp_cpu_t send;
	eacp_response_t resev;
	printf("cpu_high_clear\n");
	
	//...
	send.eacp_hdr.preamble = 0x50434145;
	send.eacp_hdr.sequence = sequence_gene();
	send.eacp_hdr.msghdr.msgType = 0x0240;
	send.eacp_hdr.msghdr.msgLen = 0x0400;
	send.cpu = cpu;	
	
	
     
	if(octpow_cmd_send_recv(&send,sizeof(send),&resev,sizeof(resev))<0) 
        return -1;
    
    return 0xffffffff;
}
