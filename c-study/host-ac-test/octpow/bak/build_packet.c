#include "build_packet.h"

int32_t build_packet(cvmx_eacp_packet_t  *buf)
{
        int i = 0;
        printf("build_packet.\n");
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

int32_t help()
{
	printf("send_packet  send a packet\nhelp         show all the help\n"); 
}

int32_t data_plan_reset(cvmx_eacp_packet_t *buf)
{
	printf("data_plan_reset\n");
}

int32_t ac_add()
{
	printf("ac_add\n");
}

int32_t ac_del()
{
	printf("ac_del\n");
}

int32_t ac_request()
{
	printf("ac_request\n");
}

int32_t port_set()
{
	printf("port_set\n");
}

int32_t port_request()
{
	printf("port_request\n");
}

int32_t mini_jumbo_set()
{
	printf("mini_jumbo_set\n");
}

int32_t wtp_add()
{
	printf("wtp_add\n");
}

int32_t wtp_del()
{
	printf("wtp_del\n");
}

int32_t wtp_del_all()
{
	printf("wtp_del_all\n");
}

int32_t wtp_request_all()
{
	printf("wtp_request_all\n");
}

int32_t wtp_request_serial()
{
	printf("wtp_request_serial\n");
}

int32_t wtp_request()
{
	printf("wtp_request\n");
}
int32_t sta_add()
{
	printf("sta_add\n");
}

int32_t sta_del_all()
{
	printf("sta_del_all\n");
}

int32_t sta_del()
{
	printf("sta_del\n");
}

int32_t sta_request_all()
{
	printf("sta_request_all\n");
}

int32_t sta_request_serial()
{
	printf("sta_request_serial\n");
}

int32_t sta_request()
{
	printf("sta_request\n");
}

//WTP的ACL
int32_t wtp_acl_add()
{
	printf("wtp_acl_add\n");
}

int32_t wtp_acl_del_all()
{
	printf("wtp_acl_del_all\n");
}

int32_t wtp_acl_del()
{
	printf("wtp_acl_del\n");
}

int32_t wtp_acl_request_all()
{
	printf("wtp_acl_request_all\n");
}

int32_t wtp_acl_request_serial()
{
	printf("wtp_acl_request_serial\n");
}

int32_t cal_request_wtp()
{
	printf("cal_request_wtp\n");
}

//STA的ACL
int32_t sta_acl_add()
{
	printf("sta_acl_add\n");
}

int32_t sta_acl_del_all()
{
	printf("sta_acl_del_all\n");
}

int32_t sta_acl_del()
{
	printf("sta_acl_del\n");
}

int32_t sta_acl_request_all()
{
	printf("sta_acl_request_all\n");
}

int32_t sta_acl_request_serial()
{
	printf("sta_acl_request_serial\n");
}

int32_t cal_request_sta()
{
	printf("cal_request_sta\n");
}

int32_t cpu_set()
{
	printf("cpu_set\n");
}

int32_t data_plan_run_time()
{
	printf("data_plan_run_time\n");
}

int32_t data_plan_cpu()
{
	printf("data_plan_cpu\n");
}

int32_t data_plan_packet()
{
	printf("data_plan_packet\n");
}

int32_t cpu_high_alert()
{
	printf("cpu_high_alert\n");
}

int32_t cpu_high_clear()
{
	printf("cpu_high_clear\n");
}
