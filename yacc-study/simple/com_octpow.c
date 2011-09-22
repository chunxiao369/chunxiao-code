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

    uint64_t  u64;
    struct {
        uint64_t    opcode:16;  /**< Opcode for the return packet  */
        uint64_t    param:8;    /**< Opcode Specific parameters */
        uint64_t    dport:6;    /**< Desired destination port for result */
        uint64_t    rlenssz:14; /**< Size of Expected result OR no. of entries in scatter list */
        uint64_t    scatter:1;  /**< Scatter indicator  1=scatter */
        uint64_t    rid:19;     /**< Request ID  */
    }s;

} cvmx_pci_inst_irh_t;



static int32_t com_opcode_base=0x40;


static int32_t build_header(char   *buf,uint32_t opcode)
{
    cvmx_pci_inst_irh_t *irh;

    memset(buf,0,16);
    buf[12] = (STAP_ETH_P & 0xFF00)>>8;
    buf[13] = (STAP_ETH_P & 0x00FF);

    irh = (cvmx_pci_inst_irh_t *)(buf+16);
    memset(irh,0,sizeof(cvmx_pci_inst_irh_t));
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

static int32_t octpow_cmd_send_recv(void * buf, int32_t size, void *resp,int32_t outszie)
{
    int32_t recvlen;
    
    int32_t  i; 
    uint8_t   *p=buf; 
    for(i=0;i<size;i++)
	{
		if (i > 0 && i % 16 == 0)
				   printf("\n");
        printf("%02x ",p[i]&0xff);
	}
	printf("=====size:%d ========\n",size); 
       
    if(sendto(psfd_pow, buf, size , 0, (struct sockaddr*)&sll_pow, sizeof(sll_pow)) < 0) 
    {
        perror("sendto");
        return(-1);
    }

    recvlen = recvfrom(psfd_pow, resp, outszie, 0, NULL, NULL);
    /*int32_t  i; 
       uint8_t   *p=(uint8_t   *)resp; 
       for(i=0;i<recvlen;i++) 
           printf("%02x",p[i]&0xff);
       printf("]]]]]size=%d[[[[[[[\n",recvlen); 
       */

    return recvlen;
}


static int32_t  octpow_intf_close(void)
{
    return close(psfd_pow);
}

static int32_t octpow_five_tuple(int32_t rule_index,struct ST_class_entry_t* class_entry_p, uint64_t opcode )
{
    pa_stap_five_tuple_front_t front;
    pa_stap_five_tuple_resp_t  resp;

    memset(&front, 0, sizeof(pa_stap_five_tuple_front_t));
    memset(&resp, 0 , sizeof(pa_stap_five_tuple_resp_t));
    build_header(front.instr,PA_STAP_OPCODE_FIVE_TUPLE);
    front.opcode = opcode;
    front.rule_id = rule_index;

    front.five_tuple.sip      = class_entry_p->sip;
    front.five_tuple.dip      = class_entry_p->dip;
    front.five_tuple.sport    = class_entry_p->min_sport;
    front.five_tuple.dport    = class_entry_p->min_dport;
    front.five_tuple.protocol = class_entry_p->protocol;
    if (class_entry_p->key_mask & KEY_MASK_SIP)
        front.five_tuple.field_mask |= PA_STAP_5T_SIP;
    if (class_entry_p->key_mask & KEY_MASK_DIP)
        front.five_tuple.field_mask |= PA_STAP_5T_DIP;
    if (class_entry_p->key_mask & KEY_MASK_SPORT)
        front.five_tuple.field_mask |= PA_STAP_5T_SPORT;
    if (class_entry_p->key_mask & KEY_MASK_DPORT)
        front.five_tuple.field_mask |= PA_STAP_5T_DPORT;
    if (class_entry_p->key_mask & KEY_MASK_PROTOCOL)
        front.five_tuple.field_mask |= PA_STAP_5T_PROTOCOL;

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
}

static int32_t octpow_user_define(int32_t rule_index,struct ST_class_entry_t* class_entry_p, uint64_t opcode )
{
    pa_stap_ud_rule_front_t front;
    pa_stap_ud_rule_resp_t  resp;

    memset(&front, 0, sizeof(pa_stap_ud_rule_front_t));
    memset(&resp, 0 , sizeof(pa_stap_ud_rule_resp_t));
    build_header(front.instr,PA_STAP_OPCODE_UD_RULE);
    front.opcode = opcode;
    front.rule_id = rule_index;

    /*ud_num_mask is the used ud of the rule.*/
    front.ud.ud_num_mask = 0x01;
    //front.ud.data =  class_entry_p->uds[0].data;
    //front.ud.mask =  class_entry_p->uds[0].mask;

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
}

static int32_t octpow_signal_protocol(int32_t rule_index,struct ST_class_entry_t* class_entry_p, uint64_t opcode )
{
    pa_stap_sigproto_front_t front;
    pa_stap_sigproto_resp_t  resp;

    memset(&front, 0, sizeof(pa_stap_sigproto_front_t));
    memset(&resp, 0 , sizeof(pa_stap_sigproto_resp_t));
    build_header(front.instr,PA_STAP_OPCODE_SIGPROTO);
    front.opcode = opcode;
    front.rule_id = rule_index;
    
    if (class_entry_p->key_mask & KEY_MASK_SIGPROTO)
    {
        front.field_mask |= PA_STAP_FIELD_MASK_SIGPRO;
        front.sigproto = class_entry_p->sigproto;
        bcm_debug_print("sig pro: %x. \n",front.sigproto);
    }
    if (class_entry_p->key_mask & KEY_MASK_OPC)
    {
        front.field_mask |= PA_STAP_FIELD_MASK_OPC;
        front.opc = class_entry_p->opc;
        bcm_debug_print("opc: %x. \n",front.opc);
    }
    if (class_entry_p->key_mask & KEY_MASK_DPC)
    {
        front.field_mask |= PA_STAP_FIELD_MASK_DPC;
        front.dpc = class_entry_p->dpc;
        bcm_debug_print("dpc: %x. \n",front.dpc);
    }
    
    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
}

/***********************************************************
*
*                                         STAP command 
*
************************************************************/
int32_t octpow_set_config_start(void)
{
    pa_stap_set_config_front_t front;
    pa_stap_set_config_resp_t resp;
    
    build_header(front.instr,PA_STAP_OPCODE_SET_CONFIG);

    front.config_mask = PA_STAP_CONFIG_MASK_INIT;
    front.config.flow_bucket = 18;
    front.config.flow_number = 12000000;
    front.config.timer_tick = 4000;
    front.config.timer_max_tick = 100;
    front.config.maintain_once_time = 10;
    front.config.flow_expire_time = 60;

	printf("The octpow_set_config_start end!\n");

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
}

int32_t octpow_del_rule_all(void)
{
    pa_stap_set_config_front_t front;
    pa_stap_set_config_resp_t resp;
    
    build_header(front.instr,PA_STAP_OPCODE_SET_CONFIG);

    front.config_mask = PA_STAP_CONFIG_MASK_DEL_RULE_ALL;
    front.config.timer_tick = 4000;
    front.config.timer_max_tick = 100;
    front.config.maintain_once_time = 10;
    front.config.flow_expire_time = 60;

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
}

int32_t octpow_set_config(int32_t config_mask,int32_t config_value)
{
    pa_stap_set_config_front_t front;
    pa_stap_set_config_resp_t resp;
    
    build_header(front.instr,PA_STAP_OPCODE_SET_CONFIG);

    front.config_mask = config_mask;

    if(config_mask & PA_STAP_CONFIG_MASK_MODE_CHANGE)
    {
    /*
        front.config.app_specific_flag= config_value;
        printf("mode change!\n");
        */
    }

    if(config_mask & PA_STAP_CONFIG_MASK_TIME_STAMP)
    {
        front.config.time_stamp_flag= config_value;
    }

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) return -1;
    
    return resp.status&0xffffffff;
}

int32_t octpow_set_user_define( pa_stap_set_ud_t *p_ud)
{
    pa_stap_set_ud_front_t front;
    pa_stap_set_ud_resp_t resp;
    
    build_header(front.instr,PA_STAP_OPCODE_SET_UD);

    memcpy(&front.ud, p_ud, sizeof(pa_stap_set_ud_t));

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) return -1;
    
    return resp.status&0xffffffff;
}

int32_t octpow_add_rule(int32_t rule_index ,struct ST_class_entry_t* class_entry_p)
{
    int32_t rv = 0;

    if ( (class_entry_p->key_mask == 0) && (class_entry_p->user_data_mask == 0) )
    {
        return ST_INVLD_PRM;
    }

    if (class_entry_p->user_data_mask != 0)
    {
        rv = octpow_user_define(rule_index, class_entry_p, PA_STAP_CONDITION_OPCODE_ADD_VALUE);
    }
    else if (class_entry_p->key_mask & KEY_MASK_SIGALL)
    {
        rv = octpow_signal_protocol(rule_index, class_entry_p, PA_STAP_CONDITION_OPCODE_ADD_VALUE);
    }
    else if (class_entry_p->key_mask & KEY_MASK_ALL)
    {
        rv = octpow_five_tuple(rule_index,class_entry_p,PA_STAP_CONDITION_OPCODE_ADD_VALUE);
    }

    return rv;
}

int32_t octpow_delete_rule(int32_t rule_index,struct ST_class_entry_t* class_entry_p)
{
    int32_t rv = 0;

    if ( (class_entry_p->key_mask == 0) && (class_entry_p->user_data_mask == 0))
    {
        return ST_INVLD_PRM;
    }

    if (class_entry_p->user_data_mask != 0)
    {
        rv = octpow_user_define(rule_index, class_entry_p, PA_STAP_CONDITION_OPCODE_DEL_VALUE);
    }
    else if (class_entry_p->key_mask & KEY_MASK_SIGALL)
    {
        rv = octpow_signal_protocol(rule_index, class_entry_p, PA_STAP_CONDITION_OPCODE_DEL_VALUE);
    }
    else if(class_entry_p->key_mask & KEY_MASK_ALL)
    {
        rv = octpow_five_tuple(rule_index,class_entry_p,PA_STAP_CONDITION_OPCODE_DEL_VALUE);
    }

    return rv;
}

int32_t octpow_set_rule_param(int32_t rule_index,int32_t param_mask, int32_t param_value)
{
    pa_stap_set_rule_front_t front;
    pa_stap_set_rule_resp_t resp;

    build_header(front.instr,PA_STAP_OPCODE_SET_RULE);
    front.rule_id= rule_index ;
    front.param_mask= param_mask;
    front.param.param_value = param_value;
    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;

    return resp.status&0xffffffff;

}

int32_t octpow_del_rule_param(int32_t rule_index)
{
    pa_stap_set_rule_front_t front;
    pa_stap_set_rule_resp_t resp;

    build_header(front.instr,PA_STAP_OPCODE_SET_RULE);
    front.rule_id= rule_index ;
    front.param_mask= PA_STAP_RULE_PARAM_DEL_RULE;

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;

    return resp.status&0xffffffff;
}

int32_t octpow_set_rule_action(int32_t rule_index,struct ST_forward_action_t *forward_action)
{
    pa_stap_set_rule_front_t front;
    pa_stap_set_rule_resp_t resp;
    int32_t forward_mode = forward_action->channels[0].forward_mode;

    build_header(front.instr,PA_STAP_OPCODE_SET_RULE);
    front.rule_id = rule_index;
    front.param_mask = PA_STAP_RULE_PARAM_MASK_ACTION;

    front.param.action.dispatch_mode = forward_mode;
    
    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;

    return resp.status&0xffffffff;
}

int32_t octpow_get_rule(int32_t rule_index,pa_stap_rule_info_t *stap_rule_info)
{
    pa_stap_get_rule_front_t front;
    pa_stap_get_rule_resp_t resp;
    
    build_header(front.instr,PA_STAP_OPCODE_GET_RULE);

    front.rule_id= rule_index;
    front.clear_counter= 0;

    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) return -1;

    if( stap_rule_info!=NULL ) *stap_rule_info=resp.info;
    
    return resp.status&0xffffffff;
}

/*static int32_t comp(const void* a,const void* b)
{
    uint64_t aa,bb;

    aa=*(uint64_t *)a;
    bb=*(uint64_t *)b;

    if(aa==bb) return 0;
    
    if(aa>bb)
        return 1;
    else 
        return -1;
}*/
    
int32_t octpow_ingroup_change(uint64_t rule_id, uint64_t *inportmap)
{
    uint32_t i = 0;
    pa_stap_rule_ingroup_front_t front;
    pa_stap_rule_ingroup_resp_t  resp;

    build_header(front.instr,PA_STAP_OPCODE_INGRP_CHANGE);
    front.rule_id = rule_id;
    bcm_debug_print("rule_id: %lu. \n", rule_id);
    for( i = 0; i < PORTMAP_ARRAY; i++)
    {
        bcm_debug_print("inport %d map: %016lx", i, inportmap[i]);
        front.inportmap[i] = inportmap[i];
    }
    bcm_debug_print("\n");
    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;

}

int32_t octpow_outgroup_change(uint64_t rule_id, struct ST_forward_action_t *forward_action)
{
    pa_stap_rule_outgroup_front_t front;
    pa_stap_rule_outgroup_resp_t  resp;
    uint64_t outportmap;
    
    build_header(front.instr,PA_STAP_OPCODE_OUTGRP_CHANGE);
    outportmap = forward_action->channels[0].port_map;
    bcm_debug_print("rule id: %lu. \n", rule_id);
    front.rule_id = rule_id;
    //front.outportmap = xy_cli_port_map(outportmap);
    front.dispatch_mode = forward_action->channels[0].forward_mode;
    bcm_debug_print("outport map: %016lx\n", outportmap);
    
    if(octpow_cmd_send_recv(&front,sizeof(front),&resp,sizeof(resp))<0) 
        return -1;
    
    return resp.status&0xffffffff;
}

/***********************************************************
*
*                      OCTEON  
*
************************************************************/

int32_t octpow_Initialize(void)
{
    int32_t rv;
    
    octpow_intf_init();
	printf("octpow_intf_init() end!\n");
    rv = octpow_set_config_start();    
	printf("send and receive success!\n");
    return rv; 
}

int32_t octpow_Finalize(void)
{
    return octpow_intf_close();
}

int main()
{
    int32_t r_v;

//初始化时发送了一次。
	r_v = octpow_Initialize();
//然后又发送一个命令。
    //r_v = octpow_del_rule_all(); 
    r_v = octpow_Finalize();
	return 0;
}

