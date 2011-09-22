%{
/*******************************************************************/
//
//
//
/*******************************************************************/
#include "./octpow/com_octpow_sendpacket.h"
//#include "./octpow/build_packet.h"
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
//unsigned int flag = -1;

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywarp()
{
	return 1;
}

start_cli()
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

%token TOKHEAT TOKTARGET TOKTEMPERATURE SEND PACKET
%token HELP EXIT EOS
%token ALL DATA_PLAN_RESET ADD DEL SHOW SET AC WTP STA IP MAC TYPE CTR_PORT DATA_PORT MINI_JUMBO INDEX VLAN_ID RID 
%token ACL RUNTIME PORT CPU CAPWAP INFO STATE

%union {
	char *sptr;
	void *vptr;
	unsigned int ipaddr;
	unsigned int ival;
	unsigned char macaddr[6];
}

%token <ival> _INT_
%token <ipaddr> _IPADDR_
%token <macaddr> _MACADDR_


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
		enter
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
		;

heat_switch:
		TOKHEAT STATE
		{
			//printf("\tHeat turned %s\n",$2);
		}

send_packet:
		SEND PACKET
		{
			//octpow_one_msg_start();
			printf("\nCLI>");
		}
help:
		HELP EOS
		{
			help();
            printf("\nCLI>");
		}
enter:
		EOS
		{
			printf("\nCLI>");
		}
target_set:
		TOKTARGET TOKTEMPERATURE
		{
			printf("\tTemperature Set\n");
		}
data_plan_reset:
		DATA_PLAN_RESET _INT_ EOS
		{
			printf("%d\n",$2);
			data_plan_reset();
			printf("\nCLI>");			
		}
ac_add:
		ADD AC _INT_ IP _IPADDR_ MAC _MACADDR_ EOS
		{
			ac_add($3, $5, $7);
			printf("\nCLI>");
		}
ac_del:
		DEL AC _INT_ IP _IPADDR_ MAC _MACADDR_ EOS
		{
			ac_del();
			printf("\nCLI>");
		}
ac_request:
		SHOW AC INFO EOS
		{
			ac_request();
			printf("\nCLI>");
		}
port_set:
		SET CAPWAP CTR_PORT _INT_ DATA_PORT _INT_ EOS
		{
			port_set();
			printf("\nCLI>");
		}

port_request:
		SHOW CAPWAP PORT EOS
		{
			port_request();
			printf("\nCLI>");
		}

mini_jumbo_set:
		SET MINI_JUMBO STATE EOS
		{
			mini_jumbo_set();
			printf("\nCLI>");
		}

wtp_add:
		ADD WTP _INT_ IP _IPADDR_ MAC _MACADDR_ INDEX _INT_ VLAN_ID _INT_ EOS
		{
			wtp_add();
			printf("\nCLI>");
		}

wtp_del:
		DEL WTP _INT_ MAC _MACADDR_ EOS
		{
			wtp_del();
			printf("\nCLI>");
		}

wtp_del_all:
		DEL WTP ALL EOS
		{
			wtp_del_all();
			printf("\nCLI>");
		}

wtp_request_all:
		SHOW WTP ALL INFO EOS
		{
			wtp_request_all();
			printf("\nCLI>");
		}

wtp_request_serial:
		SHOW WTP INDEX _INT_ _INT_ EOS
		{
			wtp_request_serial();
			printf("\nCLI>");
		}

wtp_request:
		SHOW WTP _INT_ MAC _MACADDR_ EOS
		{
			wtp_request();
			printf("\nCLI>");
		}

sta_add:
		ADD STA _INT_ IP _IPADDR_ MAC _MACADDR_ INDEX _INT_ VLAN_ID _INT_ MAC _MACADDR_ RID _INT_ EOS
		{
			sta_add();
			printf("\nCLI>");
		}

sta_del_all:
		DEL STA ALL EOS
		{
			sta_del_all();
			printf("\nCLI>");
		}

sta_del:
		DEL STA _INT_ MAC _MACADDR_ EOS
		{
			sta_del();
			printf("\nCLI>");
		}

sta_request_all:
		SHOW STA ALL INFO EOS
		{
			sta_request_all();
			printf("\nCLI>");
		}

sta_request_serial:
		SHOW STA INDEX _INT_ _INT_ EOS
		{
			sta_request_serial();
			printf("\nCLI>");
		}

sta_request:
		SHOW STA _INT_ MAC _MACADDR_ EOS
		{
			sta_request();
			printf("\nCLI>");
		}
		
		
		
		
		
		
wtp_acl_add:
		ADD ACL WTP TYPE _INT_ _INT_ MAC _MACADDR_ EOS
		{
			wtp_acl_add();
			printf("\nCLI>");
		}

wtp_acl_del_all:
		DEL ACL WTP ALL EOS
		{
			wtp_acl_del_all();
			printf("\nCLI>");
		}

wtp_acl_del:
		DEL ACL WTP _INT_ MAC _MACADDR_ EOS
		{
			wtp_acl_del();
			printf("\nCLI>");
		}

wtp_acl_request_all:
		SHOW WTP ALL ACL EOS
		{
			wtp_acl_request_all();
			printf("\nCLI>");
		}

wtp_acl_request_serial:
		SHOW WTP _INT_ _INT_ ACL EOS
		{
			wtp_acl_request_serial();
			printf("\nCLI>");
		}

cal_request_wtp:
		SHOW ACL _INT_ WTP MAC _MACADDR_ EOS
		{
			cal_request_wtp();
			printf("\nCLI>");
		}


sta_acl_add:
		ADD ACL STA TYPE _INT_ _INT_ MAC _MACADDR_  EOS
		{
			sta_acl_add();
			printf("\nCLI>");
		}

sta_acl_del_all:
		DEL ACL STA ALL EOS
		{
			sta_acl_del_all();
			printf("\nCLI>");
		}

sta_acl_del:
		DEL ACL STA _INT_ MAC _MACADDR_ EOS
		{
			sta_acl_del();
			printf("\nCLI>");
		}

sta_acl_request_all:
		SHOW STA ALL ACL EOS
		{
			sta_acl_request_all();
			printf("\nCLI>");
		}

sta_acl_request_serial:
		SHOW STA _INT_ _INT_ ACL EOS
		{
			sta_acl_request_serial();
			printf("\nCLI>");
		}

cal_request_sta:
		SHOW ACL _INT_ STA MAC _MACADDR_ EOS
		{
			cal_request_sta();
			printf("\nCLI>");
		}








cpu_set:
		SET CPU _INT_ EOS
		{
			cpu_set();
			printf("\nCLI>");
		}

data_plan_run_time:
		SHOW RUNTIME EOS
		{
			data_plan_run_time_response();
			printf("\nCLI>");
		}

data_plan_cpu:
		SHOW CPU EOS
		{
			data_plan_cpu();
			printf("\nCLI>");
		}

data_plan_packet:
		SHOW PACKET EOS
		{
			data_plan_packet();
			printf("\nCLI>");
		}
		;
