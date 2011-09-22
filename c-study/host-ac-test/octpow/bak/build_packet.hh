#ifndef __BUILD_PACKET__
#define __BUILD_PACKET__

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

typedef struct  {
        uint64_t  eacp_preamble:32;
        uint64_t  sequence:32;
        uint64_t  class:8;
        uint64_t  A:1;
        uint64_t  type_eacp:7;
        uint64_t  length_eacp:16;

        uint64_t  type_msg:16;
        uint64_t  length_msg:16;
    
		//EACP_HDR eacp_hdr;
        uint64_t  array[4];

} cvmx_eacp_packet_t;


//messages define

// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                     EACP Preamble                             | 
//|0 1 0 0 0 1 0 1 0 1 0 0 0 0 0 1 0 1 0 0 0 0 1 1 0 1 0 1 0 0 0 0|
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                     sequence                                  | 
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|   class       |A|   type      |      length                   | 
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                     Palyload                                  |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//class A and type make up of msg id. A means ACK. if this bit is set it mean this is a ACK 
//message to the cmd

/*基本控制消息类*/
#define BASIC_CMD_CLS	0x0000

/*清除数据面所有配置*/
#define RESET_DATAPLANE_CMD 			(BASIC_CMD_CLS |0x01)

/*添加AC的IP地址，地址可能会有多个
 *消息体内容包括: 地址数量count (1byte),IP(4bytes),MAC(6Bytes)*/
#define ADD_AC_IP_MAC_CMD			(BASIC_CMD_CLS |0x02)

/*删除AC的IP地址，地址可能会有多个
 *消息体内容包括: 地址数量count (1byte),IP(4bytes),MAC(6Bytes) */
#define DEL_AC_IP_MAC_CMD 			(BASIC_CMD_CLS |0x03)

/*查询AC的IP地址。消息体内容为空*/
#define QUERY_AC_IP_MAC_CMD 			(BASIC_CMD_CLS |0x04)

/*设置CAPWAP端口
 *消息体内容包括: 控制报文端口(2bytes),业务报文端口(2bytes) */
#define SET_CAPWAP_PORT_CMD 			(BASIC_CMD_CLS |0x05)

/*查询CAPWAP端口。消息体内容为空*/ 
#define QUERY_CAPWAP_PORT_CMD 			(BASIC_CMD_CLS |0x06)

/*设置Jumbo开关
 *消息体内容包括: Jumbo开关标识，1Byte。0表示不支持Jumbo，1表示支持Jumbo */
#define SET_JUMBO_CMD 				(BASIC_CMD_CLS |0x07)

/*添加更新WTP信息
 *消息体内容包括: 
 	Count ， WTP信息的数量，2Bytes, 一次可以下发一个WTP的信息，也可以下发多个WTP的信息。
	IP，WTP的IP地址，4Bytes。
	MAC, WTP的MAC地址，6Bytes。
	WTP Index，WTP的索引号，2Bytes。
	VLAN ID， WTP的VLAN 号，2Bytes。
 */
#define ADD_WTP_CMD 				(BASIC_CMD_CLS |0x08)

/*删除指定的WTP信息
 *消息体内容包括:
 	Count ， WTP信息的数量，2Bytes。
	MAC, WTP的MAC地址，6Bytes。
*/
#define DEL_SPECIFIED_WTP_CMD 			(BASIC_CMD_CLS |0x09)

/*删除全部WTP信息。消息体内容为空*/ 
#define DEL_ALL_WTP_CMD				(BASIC_CMD_CLS |0x0A)

/*删除全部WTP信息。消息体内容为空*/
#define QUERY_ALL_WTP_CMD 			(BASIC_CMD_CLS |0x0B)

/*查询某一段WTP的信息
 *消息体内容包括: 
	start index, 2Bytes。起始索引号。
	end index， 2Bytes。结束索引号。 
*/
#define QUERY_SECTION_WTP_CMD 			(BASIC_CMD_CLS |0x0C)

/*查询指定的WTP的信息
 *消息体内容包括: 
	Count ， WTP信息的数量，2Bytes。
	MAC, WTP的MAC地址，6Bytes。
*/
#define QUERY_SPECIFIED_WTP_CMD 		(BASIC_CMD_CLS |0x0D)

/*添加更新STA信息
 *消息体内容包括:
	Count ， STA的数量，2Bytes。
	IP，STA的IP地址，4Bytes。
	STA MAC, STA的MAC地址，6Bytes。
	STA Index, STA的索引号，2Bytes。
	VLAN ID，STA的VLAN 号，2Bytes。
 	WTP MAC, WTP的MAC地址，用于封装CAPWAP隧道，STA，WTP的信息都以MAC地址为索引建表。可以通过WTP的MAC查询WTP的其它信息。6Bytes。
	RID，Radio ID，1Byte。
*/
#define ADD_STA_CMD 				(BASIC_CMD_CLS |0x0E)

/*删除全部STA信息。消息体内容为空 */
#define DEL_ALL_STA_CMD 			(BASIC_CMD_CLS |0x0F)

/*删除指定STA信息。删除以MAC地址指定的STA的信息。
 *消息体内容包括: 
	Count ， STA信息的数量，2Bytes。
	MAC, STA的MAC地址，6Bytes。
*/
#define DEL_SPECIFIED_STA_CMD 			(BASIC_CMD_CLS |0x10)

/*查询全部STA信息。消息体内容为空 */
#define QUERY_ALL_STA_CMD 			(BASIC_CMD_CLS |0x11)

/*查询某一段STA信息
 *消息体内容包括: 
	start index, 2Bytes。起始索引号。
	end index，2Bytes。结束索引号。
*/
#define QUERY_SECTION_STA_CMD 			(BASIC_CMD_CLS |0x12)

/*查询指定的STA的信息
 *消息体内容包括: 
	Count ， STA信息的数量，2Bytes, 一次可以查询一个STA的信息，也可以查询多个STA的信息。
	MAC, STA的MAC地址，6Bytes。
*/
#define QUERY_SPECIFIED_STA_CMD 		(BASIC_CMD_CLS |0x13)

/*设置数据面Keep-Alive时间间隔
 *消息体内容包括:
 *	keep-alive time,2Bytes。KeepAlive时间间隔，单位 秒。 
 */
#define SET_KEEP_ALIVE_INTERVAL_CMD 		(BASIC_CMD_CLS |0x14)

/*设置数据面数据转发模式
 *消息体内容包括:
mode,1Bytes。数据转发模式。
0 – 智能转发。数据面根据业务数据的类型判断转发模式，如果收到的包是CAPWAP封装的数据报文，则是集中转发模式，解开CAPWAP头转发出去，从BRAS回来的包加上CAPWAP头发到WTP。如果收到的业务报文不带CAPWAP头，则是非集中转发模式，上下行的包都不需要进行CAPWAP处理。
1– 集中转发模式。
2 - 非集中转发模式。
其它模式以后根据需要定义。
 *
 */
#define SET_DATAPLANE_FORWARD_MODE_CMD 		(BASIC_CMD_CLS |0x15)

/*ACL 相关消息类*/
#define ACL_CMD_CLS		0x0100

/*添加更新WTP的ACL
 *消息体内容包括:
 *	type, ACL的类型，1Byte。0表示黑名单（拒绝访问列表），1表示白名单（允许访问列表）。ACL类型是两种类型中的一种，不能两种类型的列表都存在。如果是黑名单，则黑名单之外的WTP都能访问。如果是白名单，则仅白名单内的WTP可以访问，其它的不能访问。如果下发的类型与之前的ACL列表的类型不一致，则删除之前的ACL列表，以新的类型建立新的ACL列表。如果与之前的ACL类型一致，则添加至原有列表中。
	Count ， WTP的ACL条目数，2Bytes。
	MAC, WTP的MAC地址，6Bytes。
*/
#define ADD_WTP_ACL_CMD 			(ACL_CMD_CLS|0x01)

/*删除全部WTP的ACL
 *消息体内容包括:消息体内容为空 */
#define DEL_ALL_WTP_ACL_CMD 			(ACL_CMD_CLS|0x02)

/*删除ACL中指定WTP
 *消息体内容包括:
	Count ， WTP的ACL信息的数量，2Bytes。
	MAC, 要从ACL列表中删除的WTP的MAC地址，6Bytes。	
*/
#define DEL_SPECIFIED_WTP_ACL_CMD 		(ACL_CMD_CLS|0x03)

/*查询全部WTP的ACL
 *消息体内容包括:消息体内容为空 */
#define QUERY_ALL_WTP_ACL_CMD 			(ACL_CMD_CLS|0x04)

/*查询某一段WTP的ACL
 *消息体内容包括: 
	start index, 2Bytes。起始索引号。
	end index, 2Bytes。结束索引号。
*/
#define QUERY_SECTION_WTP_ACL_CMD 		(ACL_CMD_CLS|0x05)

/*查询ACL中部分指定WTP的信息
 *消息体内容包括:
	Count ， WTP信息的数量，2Bytes。
	MAC, 　WTP的MAC地址，6Bytes。	
 */
#define QUERY_SPECIFIED_WTP_ACL_CMD 		(ACL_CMD_CLS|0x06)

/*添加更新STA的ACL
 *消息体内容包括: 
	type, ACL的类型，1Byte。0表示黑名单（拒绝访问列表），1表示白名单（允许访问列表）。ACL类型是两种类型中的一种，不能两种类型的列表都存在。如果是黑名单，则黑名单之外的STA都能访问。如果是白名单，则仅白名单内的STA可以访问，其它的不能访问。如果下发的类型与之前的ACL列表的类型不一致，则删除之前的ACL列表，以新的类型建立新的ACL列表。如果与之前的ACL类型一致，则添加至原有列表中。
	Count ， STA的ACL条目数，2Bytes。
	MAC, STA的MAC地址，6Bytes。
 */
#define ADD_STA_ACL_CMD 			(ACL_CMD_CLS|0x07)

/*删除全部STA的ACL
 *消息体内容包括: 消息体内容为空 */
#define DEL_ALL_STA_ACL_CMD 			(ACL_CMD_CLS|0x08)

/*删除ACL中指定STA
 *消息体内容包括: 
	Count ， STA的ACL信息的数量，2Bytes。
	MAC, 要从ACL列表中删除的STA的MAC地址，6Bytes。
*/
#define DEL_SPECIFIED_STA_ACL_CMD 		(ACL_CMD_CLS|0x09)

/*查询全部STA的ACL
 *消息体内容包括: 消息体内容为空 */
#define QUERY_ALL_STA_ACL_CMD 			(ACL_CMD_CLS|0x0A)

/*查询某一段STA的ACL
 *消息体内容包括: 
	start index, 2Bytes。STA起始索引号。
	end index, 2Bytes。STA结束索引号。
*/
#define QUERY_SECTION_STA_ACL_CMD 		(ACL_CMD_CLS|0x0B)

/*查询ACL中部分指定的STA
 *消息体内容包括: 
	Count ， STA信息的数量，2Bytes。
	MAC, 　 STA的MAC地址，6Bytes。	
*/
#define QUERY_SPECIFIED_STA_ACL_CMD 		(ACL_CMD_CLS|0x0C)

/*QOS 相关消息类*/
#define QOS_CMD_CLS 	0x0200

/*流量控制相关消息类*/
#define TC_CMD_CLS		0x0300

/*告警设置消息类*/	
#define ALARM_SET_CMD_CLS 	0x0400

/*设置CPU利用率告警门限
 *消息体内容包括:
	CPU usage Alarm level, 4Bytes,CPU利用率告警门限值。
*/	
#define SET_CPU_ALARM_LEVEL_CMD 		(ALARM_SET_CMD_CLS|0x01)

/*统计相关消息类*/	
#define STAT_CMD_CLS 	0x2000

/*数据面运行时间。获取数据面运行时间的消息
 *消息体内容包括: 消息体内容为空。*/
#define GET_DATAPLANE_RUN_TIME_CMD 		(STAT_CMD_CLS|0x01)

/*数据面CPU平均利用率。统计数据面CPU平均利用率消息
 *消息体内容包括: 消息体内容为空 */
#define GET_DATAPLANE_CPU_USAGE_CMD 		(STAT_CMD_CLS|0x02)

/*数据面数据包统计。统计数据面的数据包信息
 *消息体内容包括: 消息体内容为空 */
#define GET_DATAPLANE_PACKETS_STAT_CMD 		(STAT_CMD_CLS|0x03)

/*告警消息类*/
#define ALARM_MSG_CLS	0x4000

/*CPU利用率过高告警消息
 *消息体内容包括: 
	CPU Usage, 4Bytes。CPU利用率过高告警时CPU利用率。
*/
#define DATAPLANE_CPU_USAGE_ALARM 		(ALARM_MSG_CLS|0x01)

/*CPU利用率过高告警清除
 *消息体内容包括: 
	CPU Usage, 4Bytes。CPU利用率过高告警清除时CPU利用率。
 */
#define DATAPLANE_CPU_USAGE_ALARM_CLEAR 	(ALARM_MSG_CLS|0x02)

#define EACP_PREAMBLE	0x45414350
#define EACP_VER(p) 		(p & 0x0000000f)
#define IS_EACP_MSG(m) 	(!((m & 0xfffffff0) ^EACP_PREAMBLE))	

#define EACP_ACK_MSG	0x0080

#define IS_EACP_ACK_MSG(m) 			(m & EACP_ACK_MSG)
#define GET_EACP_ORI_MSG(m)			(m & ~EACP_ACK_MSG)





/*msg Data structrue define*/
struct msgHdr{
	unsigned short msgType;
	unsigned short msgLen;
};

typedef struct _eacpHdr{
	unsigned int preamble;
	unsigned int sequence;
	struct msgHdr msghdr;	
}EACP_HDR;

#define EACP_HDR_LEN sizeof(struct eacpHdr)



typedef struct _addrIpMac{
	unsigned char ip_addr[4];
	unsigned char mac_addr[6];
} addrIpMacStru;

/*设置AC IP地址MAC地址消息结构*/
struct ac_addr_info_msg_stru{
	unsigned char count; /*indicate ac address number*/
	
	addrIpMacStru acAddr[0]; /*the ac address number depend on count , 
	  					  *use zero member array mean
	  					  *it does not have space in this structure*/
};

/*设置CAPWAP端口消息结构*/
struct capwapPortInfo{
	unsigned short ctlPort;
	unsigned short dataPort;
	
};

typedef struct _macStru{
	unsigned char mac[6];
}macStru;

/*回复的结果消息结构，添加删除WTP，STA, ACL
等消息的返回结果均可此结构*/
typedef struct _macResultStru{
	unsigned char mac[6];
	unsigned char result;	
}macResultStru;

/*WTP 的信息结构，为了让结构体不被编译器填充
*这里统计一用unsigned char型来定义，从网口上接收到
*一个包后用这些结构体去格式化buf，可以方便取出各
*字段的值
*/
typedef struct _wtpInfo{
	unsigned char ip_addr[4];
	unsigned char mac_addr[6];
	unsigned char wtp_index[2];  
	unsigned char wtp_vlanId[2]; /*to make sure the structure not be padded,
							    *all members use type of char */	
}wtpInfoStru;

/*设置或返回WTP信息的 消息体结构，wtpinfo是一个0元素的数组
不占结构体的空间，因为后面的长度要根据count的数量来决定，
但使用的时候同样可以用来格式化后面的数据。处理的时候
可以根据count来取后面的数据*/
struct wtp_info_msg_stru{
	unsigned short count;/*indicate the number of wtps info*/
	
	wtpInfoStru wtpInfo[0];/*the number of wtpInfo  depend on count , 
	  					  *use zero member array mean
	  					  *it does not have space in this structure*/
};

/*STA的信息结构*/
typedef struct _staInfo{
	unsigned char sta_ip[4];
	unsigned char sta_mac[6];
	unsigned char sta_index[2];
	unsigned char sta_vlanId[2];
	unsigned char assoc_wtp_mac[6];
	unsigned char wtp_rid;			/*to make sure the structure not be padded,
							    *all members use type of char */	
}staInfoStru; 

/*设置或返回STA的信息的消息结构*/
struct sta_info_msg_stru{
	unsigned short count;
	staInfoStru  staInfo[0];
};

/*设置ACL信息的消息结构
或查询全部ACL 的返回消息结构*/
struct acl_info_msg_stru{
	unsigned char acl_type;
	unsigned char count[2];    /*be care of this member, use unsigned char to make sure structure
							not be padded. it should be converted to unsigned short when use it */
	macStru mac_array[0];
};

/*查询指定的WTP或STA的ACL 的返回消息结构*/
struct acl_ack_msg_stru{
	unsigned char acl_type;
	unsigned char count[2]; /*be care of this member, use unsigned char to make sure structure
							not be padded. it should be converted to unsigned short when use it */
	macResultStru macResult[0];
};

/*msg reply in mac + result , such as reply to add del query wtp or sta info.*/
struct mac_ack_msg_stru{
	unsigned short count;/*indicate wtp number*/
	
	macResultStru macReult[0]; /*the number of wtpInfo set result  depend on count , 
	  					  		       *use zero member array mean
	  					      			*it does not have space in this structure*/
};

/*删除或查询指定的WTP或STA或ACL 的消息结构*/
struct mac_del_or_query_msg_stru{
	unsigned short count;
	macStru 	mac_array[0];
	
};

/*查询某一段WTP或STA或ACL 的消息结构*/
struct sec_stru{
	unsigned short start_index;
	unsigned short end_index;
};

/*数据统计消息结构*/
struct packets_stat_stru{
	unsigned int total_rxPackets;
	unsigned int total_rxErrorPackets;
	unsigned int total_rxDropPackets;
	unsigned int total_rxBytes;
	unsigned int total_txPackets;
	unsigned int total_txErrorPackets;
	unsigned int total_txDropPackets;
	unsigned int total_txBytes;
	unsigned int capwap_forwardPackets;
	unsigned int capwap_encapPackets;
};

/*
 * msg just has 1 byte, such as  result  jumbo , use 
 * unsignded char buff[EACP_HDR_LEN + 1]  to present the space.
 *
 * msg payload is 2 bytes or 4 bytes, such as RUN_TIME CPU_USAGE ALARM_LEVEL, 
 * just use unsigned short or unsigned int to present the space 
 */

//---------------------------------------------------------------------------------

typedef struct {
	EACP_HDR eacp_hdr;
}eacp_request_t;

typedef struct {
	EACP_HDR eacp_hdr;
    unsigned int result;
}eacp_response_t;

typedef struct {
	EACP_HDR eacp_hdr;
    unsigned int count;
	addrIpMacStru *ip_mac;
}eacp_ac_ip_mac_t;

typedef struct {
	EACP_HDR eacp_hdr;
    unsigned int control_port;
    unsigned int data_port;
}eacp_port_t;

typedef struct {
	EACP_HDR eacp_hdr;
    unsigned int mini_jumbo;
}eacp_mini_jumbo_t;

typedef struct {
	EACP_HDR eacp_hdr;
	unsigned int count;
	wtpInfoStru * wtpinfo;
}eacp_wtp_info_t;

typedef struct {
    EACP_HDR eacp_hdr;
    unsigned int count;
    macResultStru * mac_result;	
}eacp_wtp_response_t;

/*
typedef struct {
    EACP_HDR eacp_hdr;
    unsigned int count;
    macStru * mac;
}
*/
int32_t data_plan_reset();

int32_t ac_add();
int32_t ac_del();
int32_t ac_request();

int32_t port_set();
int32_t port_request();
int32_t mini_jumbo_set();

int32_t wtp_add();
int32_t wtp_del();
int32_t wtp_del_all();
int32_t wtp_request_all();
int32_t wtp_request_serial();
int32_t wtp_request();

int32_t sta_add();
int32_t sta_del_all();
int32_t sta_del();
int32_t sta_request_all();
int32_t sta_request_serial();
int32_t sta_request();

//WTP的ACL
int32_t wtp_acl_add();
int32_t wtp_acl_del_all();
int32_t wtp_acl_del();
int32_t wtp_acl_request_all();
int32_t wtp_acl_request_serial();
int32_t cal_request_wtp();

//STA的ACL
int32_t sta_acl_add();
int32_t sta_acl_del_all();
int32_t sta_acl_del();
int32_t sta_acl_request_all();
int32_t sta_acl_request_serial();
int32_t cal_request_sta();

int32_t cpu_set();
int32_t data_plan_run_time();
int32_t data_plan_cpu();
int32_t data_plan_packet();

int32_t cpu_high_alert();
int32_t cpu_high_clear();
    

int32_t build_packet(cvmx_eacp_packet_t  * buf);


#endif
