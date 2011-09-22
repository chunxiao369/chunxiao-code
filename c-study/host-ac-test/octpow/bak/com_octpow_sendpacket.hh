#ifndef __COM_OCTPOW_SENDPACKET_H__
#define __COM_OCTPOW_SENDPACKET_H__

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

/*
typedef struct  {
    uint64_t  eacp_preamble:32;	
	uint64_t  sequence:32;	
	uint64_t  class:8;
	uint64_t  A:1;
	uint64_t  type_eacp:7;
	uint64_t  length_eacp:16;	

	uint64_t  type_msg:16;
	uint64_t  length_msg:16;
    
    uint64_t  array[4];
	// point to the msg's content
	//uint32_t  ptr_msg_content;
	
} cvmx_eacp_packet_t;
*/




/***********************************************************
*
*                      OCTEON  
*
************************************************************/
//int32_t build_packet(cvmx_eacp_packet_t  * buf);

int32_t octpow_one_msg_start(void);

int32_t octpow_Initialize(void);

int32_t octpow_Finalize(void);

#endif

