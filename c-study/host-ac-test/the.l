%{
#include <stdio.h>
#include "the.tab.h"
%}

%option
integer  [0-9]+
hexdigit [0-9a-fA-F]
hexint   {hexdigit}+

ipnum    [0-9]{1,2}|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]
ipaddr   {ipnum}\.{ipnum}\.{ipnum}\.{ipnum}

macnum   {hexdigit}{2}
macaddr  {macnum}:{macnum}:{macnum}:{macnum}:{macnum}:{macnum}

%%
heat             return TOKHEAT;
target           return TOKTARGET;
temperature      return TOKTEMPERATURE;
send             return SEND;
packet           return PACKET;
on|off           yylval.ival=!strcmp(yytext,"on");return STATE;
help             return HELP;
exit             return EXIT;
acl 			 return ACL;
runtime			 return RUNTIME;
cpu 			 return CPU;
capwap			 return CAPWAP;
port 			 return PORT;
type			 return TYPE;
all		 		 return ALL;
data_plan_reset  return DATA_PLAN_RESET;
add              return ADD;
del		 		 return DEL;
show		 	 return SHOW;
set 			 return SET;
ac		 		 return AC;
wtp		 		 return WTP;
sta		 		 return STA;
ip               return IP;
mac              return MAC;
cport		 	 return CTR_PORT;
dport		 	 return DATA_PORT;
mini_jumbo       return MINI_JUMBO;
index	         return INDEX;
vlan_id          return VLAN_ID;
rid              return RID;
info		 	 return INFO;
{integer}	 {
				yylval.ival = atoi(yytext);
				return _INT_;
			 }

{macaddr}	 {
			unsigned int mac[6];

			sscanf(yytext, "%x:%x:%x:%x:%x:%x",
			&mac[0], &mac[1], &mac[2], 
			&mac[3], &mac[4], &mac[5]);

			yylval.macaddr[0] = (unsigned char)mac[0]; yylval.macaddr[1] = (unsigned char)mac[1];
			yylval.macaddr[2] = (unsigned char)mac[2]; yylval.macaddr[3] = (unsigned char)mac[3];
			yylval.macaddr[4] = (unsigned char)mac[4]; yylval.macaddr[5] = (unsigned char)mac[5];
			
			printf("mac=%s\n",yytext);
			return _MACADDR_;
		 	}

{ipaddr}	{ 
			yylval.ipaddr = ntohl(inet_addr(yytext));
			return _IPADDR_;
		 	}

[\r\n]		{
              return EOS;
            }

[ \t]+      ;

%%

int32_t yywrap(void)
{
    return 1;
}
