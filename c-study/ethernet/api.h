/* file include.h */


#ifndef _OPTIWAY_INCLUDE_H
#define _OPTIWAY_INCLUDE_H
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>



#define PRINTF(fmt, args...)	//printf(fmt , ##args)
#define GET_BIT(value, bit)	((value >> (bit)) & 1)


#define G_RULE_NUM              32
#define G_INGRP_NUM				32
#define G_OUTGRP_NUM			32 
#define RULE_STAGE_BASE			100
#define G_ENTRY_NUM_PER_GROUP  	128
#define G_ENTRY_NUM           	896
#define G_ENUM_DEFAULT		  	128
#define MULTIPLE_RULE_TO_INGRP


#define COUNTER_VERBOSE

#ifdef COUNTER_VERBOSE
#define COUNTERS	6
#else 
#define COUNTERS	2
#endif

#define SLEEP_TIME			100  /*ms*/
#define COUNTER_INTERVAL	(SLEEP_TIME*10)

#define MIB_BASE_PAGE		(uint32_t)0x50
#define MIB_TX_OCTETS		(uint32_t)0x00
#define MIB_TX_BROADCAST	(uint32_t)0x10
#define MIB_TX_MULTICAST	(uint32_t)0x14
#define MIB_TX_UNICAST		(uint32_t)0x18

#define MIB_RX_OCTETS		(uint32_t)0x44
#define MIB_RX_BROADCAST	(uint32_t)0x90
#define MIB_RX_MULTICAST	(uint32_t)0x8C
#define MIB_RX_UNICAST		(uint32_t)0x88
#define MIB_RX_OVERSIZE		(uint32_t)0x6C
#define MIB_RX_JABBERS		(uint32_t)0x70
#define MIB_RX_ALIGNMENT	(uint32_t)0x74
#define MIB_RX_FCS			(uint32_t)0x78
#define MIB_RX_FRAGMENT		(uint32_t)0x98


#define _5324_UNIT_NUM		2
#define _5324_UNIT1			1
#define _5324_UNIT2			2
#define _5324_PORTS			24
#define _56304_UNIT			0

#define PORT_ENABLE			1
#define PORT_DISABLE		0

#define MIB_COUNTER_BASE	0x50
#define NAME_LEN 			80
#define SPD_LEN				100
#define MAX_APORTS			24
#define MAX_BPORTS			MAX_APORTS
#define MAX_CPORTS			16
#define MAX_GEPORTS			(MAX_CPORTS / 2)

#define USER_DEFINE_DEFAULT	1
#define USER_UNDEFINED_DEFAULT 0
//#define _DEBUG_
/*0表示电口，1表示光口，默认ge0-7为电口*/
#define DEFAULT_PHYMODE		0 
#define CPORTS_PHYMODE_FIBER		0xff
#define PORT_BASE			1000	
#define SPD_WAIT_TIME		1000000L
#define SPD_ETH_BASE		0x997f
#define SPD_TPID			0x8100
#define SPD_XE2_PVLAN		0x64
#define GE_PVLAN_BASE		48
#define CPORTS_MASK			(((uint64_t)1<<(MAX_CPORTS+1)) -2)
#define GEPORTS_MASK		(((uint64_t)1<<(MAX_GEPORTS+1)) -2)

#define MAX_PORTS			(MAX_APORTS + MAX_BPORTS + MAX_CPORTS)
#define PORTMAP_ARRAY		(MAX_PORTS/BITS_OF_LONG +1)

#define BITS_OF_INT			(sizeof(uint32_t) * 8)
#define BITS_OF_LONG		(sizeof(uint64_t) * 8)

#define BCM_56314_INPORTS		0X0F0000	/*对应16-19口，连5324*/
#define USER_DATA_LEN			1	/* in bytes */
#define DEFAULT_USER_DATA_MASK		0x00

#define TO_STAGE2_EGRESSMASK	(0x1fffffff&(~ xy_bcm_port_map((uint64_t)1 << 26)))
#define MAX_OUT_PORT			256

#define SPD_MAX_LEN				1500
#define SPD_MIN_LEN				64
#define	SPD_PAD_BYTE			0
#define MAX_UD_ID				64
#define MAX_TRUNK_ID			31

#define CHANNEL_NUMBER			2
#define XGE1_PORT_MAP			0x1000000
#define XGE2_PORT_MAP			0x2000000

#define BOARD_EP5800			0x2014
#define LOCKA
#define UNLOCKA

#define SPD_START_BYTE			0x02
#define SPD_STOP_BYTE			0x03
#define SPD_REQUEST				0x01
#define SPD_REPLY				0x00
#define CONFIG_FILE_NAME "/appfs/config/%s/save.conf"

enum
{
	SPD_OID_RESERVED,
	SPD_OID_PLATFORM,	//part_number
	SPD_OID_NAME,
	SPD_OID_ID,		//	product_serial_number
	SPD_OID_IP,
	SPD_OID_SW_VERSION,
	SPD_OID_SPD_VERSION,
	SPD_OID_OPMODE
	
};

enum
{
	PHYMODE_COPPER,
	PHYMODE_FIBER
};


#define classify_entries(V,S)  ( (S==1) ? V->stage1_classify_entries : V->stage3_classify_entries)

struct eth_hdr
{
	uint8_t dstmac[6]; //目标mac地址
	uint8_t srcmac[6]; //源mac地址
	uint16_t eth_type; //以太网类型
};

struct eth_vlan_hdr
{
	uint8_t dstmac[6]; //目标mac地址
	uint8_t srcmac[6]; //源mac地址
	uint16_t tpid;
	uint16_t tag;
	uint16_t eth_type; //以太网类型
};


struct spd_info_t
{
	int32_t valid;
	int32_t loop;
	char platform[SPD_LEN];
	char name[SPD_LEN];
	char id[SPD_LEN];
	char ip[SPD_LEN];
	char version[SPD_LEN];
	char spd_version;
	int32_t opmode;
	int32_t port;
};

struct port_ingroup_t{
	struct port_ingroup_t *prev;
	struct port_ingroup_t *next;
	uint32_t group_id;
	char   name[NAME_LEN];
	uint64_t inportmap[PORTMAP_ARRAY];
	struct ST_rule_t* rule[G_RULE_NUM+1];
	struct ST_rule_t* stg2_rule[G_RULE_NUM+1];
};

struct port_outgroup_t{
	struct port_outgroup_t *prev;
	struct port_outgroup_t *next;
	uint32_t group_id;
	char   name[NAME_LEN];
	uint64_t outportmap;
	struct ST_rule_t* rule[G_RULE_NUM+1];
	struct ST_rule_t* stg2_rule[G_RULE_NUM+1];

};



enum {
    KEY_MODE_OFFSET_TYPE_HEAD,
    KEY_MODE_OFFSET_TYPE_L3,
    KEY_MODE_OFFSET_TYPE_L4,
    KEY_MODE_OFFSET_TYPE_TAIL
};

struct ST_user_data_t {
    int32_t id;

    int32_t offset_type;			/* head l3 l4 or tail */
    int32_t offset;

    uint8_t   data[USER_DATA_LEN];
    uint8_t   mask[USER_DATA_LEN];
};


struct ST_trunk_id_t {
    int32_t used;
    int32_t ports[MAX_OUT_PORT / BITS_OF_INT];
};


struct bcm_get_port_status_t
{
	int32_t unit;
	int32_t port;
	int32_t link;
	int32_t speed;
	int32_t duplex;
	int32_t autoneg;
	int32_t pause;
	int32_t max_frame;
};


enum {
	
    FORWARD_MODE_DROP ,
    FORWARD_MODE_STAGE2  ,
    FORWARD_MODE_RR , 	
    FORWARD_MODE_MC  ,	//MULTICAST
    FORWARD_MODE_HASH_S  ,
    FORWARD_MODE_HASH_D ,
    FORWARD_MODE_HASH_M ,

	FORWARD_MODE_HASH_OPC ,
	FORWARD_MODE_HASH_DPC ,
	FORWARD_MODE_HASH_UD ,

	FORWARD_MODE_INVALID,

    FORWARD_MODE_UNKOWN = 0xff
};

struct ST_forward_channel_t {
    /* DROP NEXT_STAGE RR HASH_S HASH_D HASH_M */
    int32_t forward_mode;
    uint64_t port_map;
    int32_t logic_port;
};

struct ST_forward_action_t {
    struct ST_forward_channel_t channels[CHANNEL_NUMBER];

//emmy    int32_t next_stage;		/* 2 or 3, 0 means no next stage */
//emmy    uint32_t mode_id;	/* keymode or strmode id, decided by next_stage */

    /* forward parameters */
//emmy    int32_t priority;
//emmy    int32_t sample_rate;
//emmy    int32_t max_packet_send;
//emmy    int32_t max_packet_send_valid;
//emmy    int32_t smac_mode;
};


struct ST_rule_t {
	struct ST_rule_t* prev;
    struct ST_rule_t* next;
    int32_t rule_id;
    char   name[NAME_LEN];

	int32_t ingroup_id;
	uint64_t inportmap[PORTMAP_ARRAY];
	int32_t outgroup_id;
    int32_t default_key_mode_id;

    /* class entry array, the size is subblock_number * ENTRY_NUMBER_PER_SUBBLOCK. */
    struct ST_class_entry_t*  class_entry_p;
    uint32_t entry_number;
	uint32_t valid_entry_number;
	uint32_t total_enum;
	uint32_t current_enum;	
    /* maybe needed in future version. */
    /* uint32_t max_class_entry; */

    /* forward action */
    struct ST_forward_action_t forward_action;

};





enum {
	PORT_SPEED_10_M_HD,
	PORT_SPEED_10_M_FD,
	PORT_SPEED_100_M_HD,
	PORT_SPEED_100_M_FD,
	PORT_SPEED_1000_M_HD,
	PORT_SPEED_1000_M_FD,
	PORT_SPEED_MAX	
};

enum {
    KEY_MASK_IPVERSION	=	0x0001,
    KEY_MASK_PROTOCOL	= 	0x0002,
    KEY_MASK_SIP		= 	0x0004,
    KEY_MASK_DIP		= 	0x0008,
    KEY_MASK_SPORT		= 	0x0010,
    KEY_MASK_DPORT		= 	0x0020,
    KEY_MASK_ETHTYPE    =   0x0040,
    KEY_MASK_SYN		= 	0x0080,
    KEY_MASK_RST		= 	0x0100,
    KEY_MASK_PSH		= 	0x0200,
    KEY_MASK_ACK		= 	0x0400,
    KEY_MASK_URG		=	0x0800,
    KEY_MASK_TCP_FLAG	= 	0x0fc0,
//    KEY_MASK_SIZE		= 	0x1000,
	KEY_MASK_OUTER_VLAN	=   0x1000,
    KEY_MASK_RANGE 		= 	0x2000,
    KEY_MASK_IPFRAGMENT = 	0x4000,
    KEY_MASK_INTERFACE	=	0x8000,
    KEY_MASK_ALL 		= 	0xefff,
    KEY_MASK_SIPV6		=	0x10000,
    KEY_MASK_DIPV6		=	0x20000,
	KEY_MASK_INPORTS	=	0x40000,
	KEY_MASK_DSTPORTTGID=	0x80000,
    KEY_MASK_SIGPROTO	=	0x100000,
	KEY_MASK_OPC		= 	0x200000,
	KEY_MASK_DPC		=	0x400000,
	KEY_MASK_SIGALL		=	0x700000	
};


struct _5324_counter_t
{
    uint32_t page;
    uint32_t addr;

    uint64_t total;
    uint64_t last;
    uint64_t rate;
	uint32_t len;
};


struct ST_ud_t {
    int32_t id;
    char data;
    char mask;
};


struct ST_serial_t {
	int32_t dst;
	int32_t flag;
};


struct _vlan_entrynum {
	uint16_t vlan_id;
	int32_t cam_entry_num;
};

struct _cport_entrynum {
	uint64_t cport_map;
	int32_t cam_entry_num;
};

struct ST_class_entry_t {
    uint32_t key_mask;
    uint32_t user_data_mask;

    uint32_t protocol;
	uint32_t protocol_mask;

    uint32_t sip;
    uint32_t sip_mask;

    uint32_t dip;
    uint32_t dip_mask;

    uint16_t min_sport;
    uint16_t max_sport;
    uint16_t sport_mask;

    uint16_t min_dport;
    uint16_t max_dport;
    uint16_t dport_mask;
	uint32_t sigproto;
	uint32_t opc;
	uint32_t dpc;

    uint32_t tcp_flag;

    uint32_t min_size;
    uint32_t max_size;

    int32_t ipfragment;
    uint64_t inports;

    int32_t ipversion;

    /* only for stage 1 */
    struct ST_ud_t uds[MAX_UD_ID];

    /* only for stage 2 */
    uint32_t key_mode_id;

    /* internal member */
    int32_t valid;

    /* BCM (gid<<16|eid) . */
//    int32_t cam_entry_number[24];
	struct _vlan_entrynum vlan_entrynum[MAX_APORTS + MAX_BPORTS+1]; 
	struct _cport_entrynum cport_entrynum;

};

struct xy_counter_t
{
        char   name[20];
        uint64_t  total;
        uint64_t  rate;
};


struct board_port_info_t {
	int32_t major_board_type;
	int32_t max_port_number;
	int32_t port_all_map;
	int32_t port_sfp_xfp_map;
	int32_t g_board_type;
	uint16_t Led_type;
}; 

struct ioctl_bcm_trunk_t{

    int32_t tid;
    int32_t rtag;
    int32_t pbmp;

};

enum {
    ST_OK,			/* ok */
	ST_NO_PRINT,
    ST_NO_MEM,		/* 1; malloc failed */
    ST_RS_FULL,		/* 2; resource full */
    ST_INVLD_PRM,		/* 3; invalid parameter */
    ST_OUT_OF_RANGE,	/* 4; value out of range */
    ST_COMM_ERR,		/* 5; common error */
    ST_FILE_ERR,		/* 6; file operate error */
    ST_INT_ERR,		/* 7; internal error */
	ST_NAME_EXT,		/* 9;rule already exist(when create) */
    ST_RULE_NOT_EXT,		/* 8; rule not exist */
    ST_RULE_NOT_EMPTY,
    ST_RULE_NO_INGRP,
    ST_RULE_ACT_FAIL,		/* 11; rule not exist */
    ST_RULE_REALLOC_ERROR,
    ST_READ_FRU_FAILED,
    ST_GET_IPADDR_FAILED,
    ST_SENDTO_SKT_FAILED,
    ST_SAVE_CFG_ERR,		/* 12;save ruleset config error */
    ST_LOAD_CFG_ERR,		/* 13;load ruleset config error */
    ST_ADD_RULE_ERR,	/* 14;add rule error */
    ST_CE_NOT_FOUND,	/* 15;class entry not found */
	ST_CE_EXT,	/* 16;class entry not found */
	ST_CE_FULL,	/* 16;class entry not found */
	ST_FREE_ENUM_SHORT,
	ST_ENUM_SHORT,
	ST_ENUM_NOT_SET,// 17
	ST_SET_ENUM_ERR,// 17
    ST_INIT_FPGA_ERR,	/* 18; initialize fpga error */
    ST_APP_FPGA_ERR,	/* 19; apply vfpga error */
    ST_INIT_CAM_ERR,	/* 20; initialize cam error */
    ST_SET_CAM_ERR,		/* 21; set cam error */
    ST_RESET_CAM_ERR,	/* 22; reset cam error */
    ST_APP_CAM_ERR,		/* 23; apply vcam error */
    ST_CAM_FULL,		/* 24; cam is full */
    ST_INIT_MOD_ERR,	/* 25; initialize module error */
    ST_MOD_NOT_EXT,		/* 26; module not exist */
    ST_WRONG_MOD,		/* 27; */
    ST_MOD_NO_ABLT,		/* 28; no ability when assign ruleset */
    ST_SAVE_MOD_ERR,	/* 29; save module config error */
    ST_LOAD_MOD_ERR,		/* 30; load module config error */
    ST_RL_FW_MODE,                 /* 29; rule action not hash  */
    ST_RL_HASH_PORT,		 /* 30; port not in hash map */
    ST_RL_HASHMAP_NO_EXT,		        /* 31; port not in hash map */
	ST_OCT_INIT_ERR,
	ST_UD_INVALID,
    ST_DEV_EXT,                 /* 32; device already exist */
	ST_DEV_NOT_EXT, 				/* 33; device NOT exist */
    ST_VST_EXT,                 /* 34; vstap already exist */
	ST_VST_NOT_EXT, 				/* 35; vstap NOT exist */
    ST_ING_EXT,                 /* 36; ingroup already exist */
	ST_ING_NOT_EXT, 				/* 38 ingroup not  exist */
	ST_INGRP_ATTACH_MUTIPLE,
	ST_INGRP_ATTACH_FULL,
	ST_INGRP_DETACH_ERR,
	ST_RULE_ALREADY_ATTACHED,
	ST_OUTG_NOT_EXT, 				/* 39; outgroup not exist */
	ST_ING_EMPT,
	ST_OUTG_EMPT,
	ST_PORTADD_MULTIPLE,
	ST_PORT_PHYMODE_ERR,
	ST_PHYMODE_NOT_AVAILABLE,
	ST_PORT_RANGE_ERROR,
	ST_INVALID_SPEED,
	ST_ABPORT_SPEED_ERROR,
	ST_PORT_NOT_SPAN,
	ST_SET_PORT_INLINE_ERR,
	ST_SET_PORT_SPEED_ERR,
	ST_CLEAR_PORT_COUNTER_ERR,
	ST_SET_PORT_AUTONEG_ERR,
	ST_PORT_NOT_COPPER
};

/* */
extern int32_t thread_run;
extern int32_t g_board_type;
extern int32_t g_opmode;
extern int32_t g_rule_stage;
extern struct ST_rule_t* p_rule_head_stg1 ;
extern struct ST_rule_t* p_rule_tail_stg1 ;
extern struct ST_rule_t* p_rule_head_stg2;
extern struct ST_rule_t* p_rule_tail_stg2;

extern struct dev_node *p_dev_head;
extern struct dev_node *p_dev_tail ;

extern struct vstap_node *p_vstap_head;
extern struct vstap_node *p_vstap_tail;
extern struct port_ingroup_t* p_ingroup_head ;
extern struct port_ingroup_t* p_ingroup_tail ;
extern struct port_outgroup_t* p_outgroup_head ;
extern struct port_outgroup_t* p_outgroup_tail ;
extern int32_t g_rule_default_act;
extern uint64_t g_rule_default_mc;
//extern int32_t g_rule_user_default;

extern struct ST_user_data_t g_udf[MAX_UD_ID];

extern  uint8_t g_port_phymode;
extern  uint8_t g_default_phymode;
extern struct spd_info_t spd_info[8];

extern  uint64_t g_port_inline[PORTMAP_ARRAY];
extern  uint64_t g_default_inline[PORTMAP_ARRAY];

extern  int64_t g_port_speed[PORT_SPEED_MAX][PORTMAP_ARRAY];
extern  int64_t g_default_speed[PORT_SPEED_MAX][PORTMAP_ARRAY];

extern  uint64_t g_port_autoneg[PORTMAP_ARRAY];
extern  uint64_t g_default_autoneg[PORTMAP_ARRAY];

extern uint64_t g_portmap[PORTMAP_ARRAY];

extern int xsfd;
extern struct sockaddr_ll sll;

/* function prototypes */


extern void api_showxfp(int32_t);


extern char  * api_get_fru_info(char   *);
extern int32_t api_get_port_info(struct board_port_info_t * boardinfo);

extern int32_t api_init_boardtype(void);

extern int32_t xy_bcm_port(int32_t);
extern uint64_t xy_bcm_port_map(uint64_t);
extern int32_t xy_cli_port(int32_t);
extern uint64_t xy_cli_port_map(uint64_t);


extern int32_t api_rule_init(void);
//extern int32_t STRl_GetEntryNum(struct ST_rule_t  * ,  int32_t * );
extern uint32_t crc32( uint32_t, const uint8_t *, uint32_t );

extern void api_show_rulesetClassifySubTable(char  *, int32_t);
extern int32_t api_add_classentry( struct ST_rule_t *, struct ST_class_entry_t*, uint32_t*);
extern int32_t api_delete_classentry( struct ST_rule_t   *,uint32_t, uint32_t);
extern int32_t api_get_classentry_index(struct ST_rule_t* , struct ST_class_entry_t*, uint32_t*);
extern int32_t api_check_classentry(struct ST_rule_t *, struct ST_class_entry_t*);
extern int32_t api_set_rule_action(struct ST_rule_t *, struct ST_forward_action_t*);
extern int32_t api_set_default_action(int32_t ,uint64_t);
extern int32_t api_show_rule_default(void);

extern int32_t api_show_rule( struct ST_rule_t *, int32_t);

//extern void * __update_linkdown_counter(int32_t, int32_t, struct _5324_counter_t * );

extern char * get_port_name(char *, int32_t );
extern int32_t api_add_ingroup_port(void *  , uint64_t *);

extern void api_spd_reset(void);

extern int32_t api_set_stg2_udf(int32_t, int32_t, int32_t);

extern int32_t api_set_port_autoneg(int32_t,int32_t );
extern int32_t api_get_port_autoneg(int32_t);

extern void api_showposfp(int32_t);
extern int32_t api_set_port_phymode(uint8_t  ,int32_t );

extern int32_t api_set_port_speed(int32_t,int32_t );

extern int32_t api_get_port_inline(int32_t);
extern void api_port_init(void);
extern int32_t api_set_port_inline(int32_t ,int32_t );
extern int api_spd_init(void);
extern int api_spd_close(void);

extern int32_t api_ingrp_init(void);
extern int32_t api_ingrp_cleanup(void);

extern int32_t api_outgrp_init(void);
extern int32_t api_outgrp_cleanup(void);
extern int32_t  set_port_phymode(uint64_t* ,int32_t);
extern int32_t api_number_of_ce(uint64_t* ,int32_t );

extern int32_t save_config( FILE* );

extern void api_opmode_cleanup(void);

extern int32_t api_saveconfig(FILE*);
extern int32_t api_initialize(void);
extern int32_t api_finalize(void);

extern int32_t api_init_phymode(void);
extern int32_t rule_init(void);
extern void rule_print(FILE *fp,void * );
extern void api_fprint_class_entry(FILE* ,struct ST_class_entry_t* , int32_t );

extern int32_t api_rule_cleanup(void);

extern int32_t init_simple_mode(void);

extern int32_t  api_clear_port_counter(uint64_t *);
extern void api_print_spd_info(uint64_t);

extern int32_t api_restore_port_config(void);
extern int32_t api_del_ingroup_port(void * , uint64_t *);

extern int32_t api_add_outgroup_port(void * , uint64_t );
extern int32_t api_del_outgroup_port(void * , uint64_t );

extern void api_ingrp_print(FILE *,void *);
extern void api_outgrp_print(FILE *, void *);
extern int32_t api_print_port_counter(int32_t , void *);

extern void api_fprint_ingrp_port(FILE *, uint64_t *,int32_t );
extern void api_fprint_outgrp_port(FILE *, uint64_t,int32_t );
extern int32_t api_set_port_autoneg(int32_t ,int32_t );
extern int32_t api_get_port_autoneg(int32_t );
extern int32_t  api_set_portmap_autoneg(uint64_t *, int32_t);
extern int32_t  api_set_portmap_speed(uint64_t *, int32_t);
extern int32_t api_set_port_inline(int32_t ,int32_t );

extern void api_set_portmap_inline(uint64_t *, int32_t );
extern void api_print_port_status(int32_t , struct bcm_get_port_status_t * );

extern int32_t number_of_map(uint64_t);
extern int32_t  get_5324unit(int32_t);

extern int32_t port_map_test(uint64_t *);
extern void* malloc(size_t);
extern int64_t strtol(const char  *, char  **, int);
extern void *FRU_List(int32_t );
extern int start_thread(void);

extern int32_t get_macaddr(char *,char *);
extern int32_t get_ipaddr(char *);
extern int32_t get_ifindex(const char *);
extern int32_t get_gateway(char *);

extern struct _5324_counter_t g_5324_counter_unit1[_5324_PORTS][COUNTERS];

extern struct _5324_counter_t g_5324_counter_unit2[_5324_PORTS][COUNTERS];
extern int32_t g_spd_auto;
extern int32_t g_spd_enable;

#endif

/* end of file include.h */
