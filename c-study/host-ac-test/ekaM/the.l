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
[0-9]+           yylval.number=atoi(yytext);return NUMBER;
heat             return TOKHEAT;
on|off           return STATE;
target           return TOKTARGET;
temperature      return TOKTEMPERATURE;
send             return TOKSEND;
packet           return TOKPACKET;
help             return HELP;
exit             return EXIT;
data_plan_reset  return DATA_PLAN_RESET;
add              return ADD;
ip               return IP;
mac              return MAC;
control_port     return CTR_PORT;
data_port        return DATA_PORT;
mini_jumbo       return MINI_JUMBO;
wtp_index        return WTP_INDEX;
vlan_id          return VLAN_ID;
sta_index        return STA_INDEX;
rid              return RID;
{integer}		{
			    	yylval.ival = atoi(yytext);
			    	return _INT_;
				}

0x{hexint}		{
					if(strlen(yytext)<=4)
					{
			    		sscanf(yytext, "0x%x", &(yylval.ival));
			    		return _INT_;
					}
					else
					{
			    		yylval.sptr = add_name_table(yytext+2 );
			    		return _HEXSTRING_;
					}
				}

{macaddr}		{
					unsigned int mac[6];

					sscanf(yytext, "%x:%x:%x:%x:%x:%x",
					&mac[0], &mac[1], &mac[2], 
					&mac[3], &mac[4], &mac[5]);

					yylval.macaddr[0] = (unsigned char)mac[0]; yylval.macaddr[1] = (unsigned char)mac[1];
					yylval.macaddr[2] = (unsigned char)mac[2]; yylval.macaddr[3] = (unsigned char)mac[3];
					yylval.macaddr[4] = (unsigned char)mac[4]; yylval.macaddr[5] = (unsigned char)mac[5];

					return _MACADDR_;
				}
{ipaddr}		{ 
			    	yylval.ipaddr = ntohl(inet_addr(yytext));
			    	return _IPADDR_;
				}
\n
.				return yytext[0];
[ \t]+
%%
int32_t yywrap(void)
{
    return 1;
}
