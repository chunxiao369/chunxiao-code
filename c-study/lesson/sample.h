enum
{
	IOCTL_BCM_GET_COUNTERS,
	IOCTL_BCM_CLEAR_COUNTERS,
	IOCTL_BCM_GET_PORT_STATUS,
	IOCTL_BCM_SET_PORT_AUTONEG,
	IOCTL_BCM_FP_INIT,
	IOCTL_BCM_FP_ENTRY_ADD,
	IOCTL_BCM_FP_UDF_SET,
	IOCTL_BCM_VLAN_INIT,
	IOCTL_BCM_VLAN_CREATE,
	IOCTL_BCM_VLAN_ADD,
	IOCTL_BCM_VLAN_REMOVE,
	IOCTL_BCM_VLAN_DESTROY,
	IOCTL_BCM_FP_ENTRY_REMOVE,
	IOCTL_BCM_PVLAN_SET,
	IOCTL_BCM_GROUP_NUMBER_GET,
	IOCTL_BCM_XGE_PHY_SET,
	IOCTL_BCM_XGE_PHY_GET,
	IOCTL_BCM_SET_PORT_LEARN,
	IOCTL_BCM_SET_PORT_PAUSE,
	IOCTL_BCM_SET_PORT_SPEED,
	IOCTL_BCM_GE_PHY_SET,
	IOCTL_BCM_GE_PHY_GET,
	IOCTL_BCM_FP_ENTRY_ACTION,
	IOCTL_BCM_TRUNK_ADD,
	IOCTL_BCM_TRUNK_REMOVE,
	IOCTL_BCM_REG_GET,
	IOCTL_BCM_REG_SET,
	IOCTL_BCM_SLB_READ,
	IOCTL_BCM_SLB_WRITE,
};

enum 
{
    KEY_MASK_IPVERSION =	 0x0001,
    KEY_MASK_PROTOCOL = 	 0x0002,
    KEY_MASK_SIP = 		     0x0004,
    KEY_MASK_DIP = 		     0x0008,
    KEY_MASK_SPORT = 		 0x0010,
    KEY_MASK_DPORT = 		 0x0020,
    KEY_MASK_FIN = 		     0x0040,
    KEY_MASK_SYN = 		     0x0080,
    KEY_MASK_RST = 		     0x0100,
    KEY_MASK_PSH = 		     0x0200,
    KEY_MASK_ACK = 		     0x0400,
    KEY_MASK_URG =		     0x0800,
    KEY_MASK_TCP_FLAG = 	 0x0fc0,
    KEY_MASK_OUTER_VLAN =    0x1000,
    KEY_MASK_RANGE = 		 0x2000,
    KEY_MASK_IPFRAGMENT = 	 0x4000,
    KEY_MASK_INTERFACE	=	 0x8000,
    KEY_MASK_ALL = 		     0xffff,
    KEY_MASK_SIPV6 =        0x10000,
    KEY_MASK_DIPV6 =        0x20000,
    KEY_MASK_INPORTS =	    0x40000,
    KEY_MASK_DSTPORTTGID =	0x80000,
};

enum
{
	KEY_MODE_TYPICAL,
	KEY_MODE_IPV6_SRC,
	KEY_MODE_IPV6_DST,
	KEY_MODE_RESERVE,
};


#define BCM_UDF_VLAN_MASK                		0x60
#define BCM_UDF_VLAN_NOTAG               		0x00
#define BCM_UDF_VLAN_ONETAG              		0x20
#define BCM_UDF_VLAN_TWOTAG              		0x40
#define BCM_UDF_VLAN_NOTUSED             		0x60

#define BCM_UDF_L2_MASK                  	    0x18
#define BCM_UDF_L2_ETHERNET2             		0x00
#define BCM_UDF_L2_SNAP                  		0x08
#define BCM_UDF_L2_LLC                   		0x10
#define BCM_UDF_L2_OTHER                 		0x18

#define BCM_UDF_IP_MASK                  		0x07
#define BCM_UDF_IP4_HDR_ONLY             		0x00
#define BCM_UDF_IP6_HDR_ONLY             		0x01
#define BCM_UDF_IP6_FRAGMENT             		0x02
#define BCM_UDF_IP4_OVER_IP4             		0x03
#define BCM_UDF_IP6_OVER_IP4             		0x04
#define BCM_UDF_IP6_FRAGMENT_OVER_IP4           0x05
#define BCM_UDF_IP_OTHER                 		0x06
#define BCM_UDF_IP_NOTUSED               		0x07

#define G_GROUP_NUM     8
#define G_RULE_NUM     64
#define G_ENTRY_NUM  1024

typedef struct
{
        char name[20];
        unsigned long long total;
        unsigned long long rate;
}xy_counter_t;

typedef struct
{
	int unit;
	int port;
	int cntr_num;
	xy_counter_t * cntr;
}ioctl_bcm_get_counters_t;

typedef struct
{
	int unit;
	int port;
}ioctl_bcm_clear_counters_t;

typedef struct
{
	int unit;
	int port;
	int link;
	int speed;
	int duplex;
	int autoneg;
	int pause;
	int max_frame;
}ioctl_bcm_get_port_status_t;

typedef struct
{
	int unit;
	int port;
	int autoneg;
	int pause;
	int speed;
	int duplex;
	int max_frame;
}ioctl_bcm_set_port_config_t;

typedef struct
{
	int unit;
	int port;
	int enable;
}ioctl_bcm_set_port_learn_t;

typedef struct
{
    unsigned int key_mask;
    //unsigned int user_data_mask;

    unsigned int protocol;
	
    unsigned int sip;
    unsigned int sip_mask;

    unsigned int dip;
    unsigned int dip_mask;
    
    unsigned short outer_vlan;
    unsigned short outer_vlan_mask;

    unsigned short min_sport;
    unsigned short max_sport;
    unsigned short sport_mask;

    unsigned short min_dport;
    unsigned short max_dport;
    unsigned short dport_mask;

    unsigned int tcp_flag;

    //unsigned char udf[32];
    //unsigned char udf_mask[32];

    int dstporttgid;
    int ipfragment;
    int inports;
    int ipversion;

    int ruleid;
    int gid;
    int eid;
}bcm_fp_entry_t ;

typedef struct
{
    int gid;
    int prio;
    int mode;
    int keymode;
    int free;
}bcm_fp_group_t ;

typedef struct
{
    int udf_id;
    int type;
    int offset;
} bcm_fp_udf_t ;

typedef struct
{
    int vid;
    unsigned int port_map;
    unsigned int untag_map;
} bcm_ext_vlan_t ;


typedef struct
{
    int vid;
    unsigned int port_map;
} bcm_pvlan_t ;

typedef struct
{
    unsigned int port;
    unsigned char devad;
    unsigned short reg;
    unsigned short mask;
    unsigned short value;
} bcm_phy_t ;


typedef struct
{
    int eid;
    int action;
    int param0;
    int param1;
} bcm_fp_entry_action_t;


typedef struct
{
    int tid;
    int rtag;
    int pbmp;
} ioctl_bcm_trunk_t;

typedef struct bcm_reg_args_s {
    char        a_cmd[64];                  /* Initial string */
    char        a_argv[16][16];      /* argv pointers */
    char        a_buffer[64];  /* Split up buffer */
    int         a_argc;                 /* Parsed arg counter */
    int         a_arg;                  /* Pointer to NEXT arg */
    int         unit;
} bcm_reg_args_t;

typedef struct bcm_slb_args_s {
    uint8_t device_id;
    uint8_t sub_mode;
    uint16_t address;
    uint16_t value;
} bcm_slb_args_t;


