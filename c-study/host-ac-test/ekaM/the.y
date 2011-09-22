%{
/*******************************************************************/
//
//
//
/*******************************************************************/
#include "./octpow/com_octpow_sendpacket.h"
#include "./octpow/build_packet.h"
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

#include "include/api.h"
#include "include/com_common_defs.h" 
#include "include/com_opcode_defs.h"
#define STAP_ETH_P 0x3322

/*******************************************************************/
//
//
//
/*******************************************************************/

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywarp()
{
	return 1;
}

start_cli()
//main()
{
	printf("yacc initialed the octpow.\n");
	octpow_Initialize();
	printf("Parse Start ...\n");
	printf("CLI>");
	yyparse();
	printf("\nParse End\n");
	octpow_Finalize();
}
%}

%token NUMBER TOKHEAT STATE TOKTARGET TOKTEMPERATURE TOKSEND TOKPACKET
%token HELP
%token EXIT

%token DATA_PLAN_RESET ADD IP MAC CTR_PORT DATA_PORT MINI_JUMBO WTP_INDEX VLAN_ID STA_INDEX RID


%union {
	char *sptr;
	void *vptr;
	unsigned int ipaddr;
	unsigned int ival;
	unsigned char macaddr[6];
}

%%
commands:  /* empty */
		| commands command
		;
command:
		heat_switch
		|
		send_packet
		|
        target_set
		|
		EXIT
		{
	    	YYACCEPT;
		}
		|
        help
		|
		data_plan_reset
		|
		ac_add
		|
		ac_del
		|
		ac_request
		|
		port_set
		|
		port_request
		|
		mini_jumbo_set
		|
		wtp_add
		|
		wtp_del
		|
		wtp_del_all
		|
		wtp_request_all
		|
		wtp_request_serial
		|
		wtp_request
		|
		sta_add
		|
		sta_del_all
		|
		sta_del
		|
		sta_request_all
		|
		sta_request_serial
		|
		sta_request
		|
		wtp_acl_add
		|
		wtp_acl_del_all
		|
		wtp_acl_del
		|
		wtp_acl_request_all
		|
		wtp_acl_request_serial
		|
		cal_request_wtp
		|
		sta_acl_add
		|
		sta_acl_del_all
		|
		sta_acl_del
		|
		sta_acl_request_all
		|
		sta_acl_request_serial
		|
		cal_request_sta
		|
		cpu_set
		|
		data_plan_run_time
		|
		data_plan_cpu
		|
		data_plan_packet
		|
		cpu_high_alert
		|
		cpu_high_clear	
		;

heat_switch:
		TOKHEAT STATE
		{
			printf("\tHeat turned on or off\n");
		}

send_packet:
		TOKSEND TOKPACKET
		{
			octpow_one_msg_start();
			printf("\nCLI>");
		}
help:
		HELP
		{
			help();
            printf("\nCLI>");
		}
target_set:
		TOKTARGET TOKTEMPERATURE NUMBER
		{
			printf("\tTemperature Set\n");
		}
data_plan_reset:
		DATA_PLAN_RESET
		{
			data_plan_reset();
			
		}

ac_add:
		ADD AC NUMBER IP MAC
		{
			ac_add();
		}
ac_del:
		DEL AC NUMBER IP MAC
		{
			ac_del();
		}
ac_request:
		
		{
			ac_request();
		}

port_set:
		{
			port_set();
		}

port_request:
		{
			port_request();
		}

mini_jumbo_set:
		{
			mini_jumbo_set();
		}

wtp_add:
		{
			wtp_add();
		}

wtp_del:
		{
			wtp_del();
		}

wtp_del_all:
		{
			wtp_del_all();
		}

wtp_request_all:
		{
			wtp_request_all();
		}

wtp_request_serial:
		{
			wtp_request_serial();
		}

wtp_request:
		{
			wtp_request();
		}

sta_add:
		{
			sta_add();
		}

sta_del_all:
		{
			sta_del_all();
		}

sta_del:
		{
			sta_del();
		}

sta_request_all:
		{
			sta_request_all();
		}

sta_request_serial:
		{
			sta_request_serial();
		}

sta_request:
		{
			sta_request();
		}

wtp_acl_add:
		{
			wtp_acl_add();
		}

wtp_acl_del_all:
		{
			wtp_acl_del_all();
		}

wtp_acl_del:
		{
			wtp_acl_del();
		}

wtp_acl_request_all:
		{
			wtp_acl_request_all();
		}

wtp_acl_request_serial:
		{
			wtp_acl_request_serial();
		}

cal_request_wtp:
		{
			cal_request_wtp();
		}


sta_acl_add:
		{
			sta_acl_add();
		}

sta_acl_del_all:
		{
			sta_acl_del_all();
		}

sta_acl_del:
		{
			sta_acl_del();
		}

sta_acl_request_all:
		{
			sta_acl_request_all();
		}

sta_acl_request_serial:
		{
			sta_acl_request_serial();
		}

cal_request_sta:
		{
			cal_request_sta();
		}

cpu_set:
		{
			cpu_set();
		}

data_plan_run_time:
		{
			data_plan_run_time();
		}

data_plan_cpu:
		{
			data_plan_cpu();
		}

data_plan_packet:
		{
			data_plan_packet();
		}

cpu_high_alert:
		{
			cpu_high_alert();
		}

cpu_high_clear:
		{
			cpu_high_clear();
		}
		;
