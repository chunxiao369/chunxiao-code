struct ethhdr_t 
{
	uint8_t		ether_dhost[ETH_ALEN];      /* destination eth addr */                                         
	uint8_t		ether_shost[ETH_ALEN];      /* source ether addr    */                                         
	uint16_t	ether_type;                 /* packet type ID field */                                         
};

struct vlan_t
{
	uint16_t	tag_control;
	uint16_t	tag_protocol;
};

struct 	iphdr_t 
{
	unsigned int   ip_hl:4;
	unsigned int   ip_v:4;
	uint8_t        ip_tos;
	uint16_t       ip_len;
	uint16_t       ip_id;
	uint16_t       ip_off;
	uint8_t        ip_ttl;
	uint8_t        ip_p;
	uint16_t       ip_sum;
	struct in_addr ip_src;
	struct in_addr ip_dst;
};

struct udphdr_t 
{
	uint16_t	source;
	uint16_t	dest;
	uint16_t	len;
	uint16_t	check;
};

typedef u_int32_t tcp_seq;

struct tcphdr_t
{
    u_int16_t th_sport;         /* source port */
    u_int16_t th_dport;         /* destination port */
    tcp_seq th_seq;             /* sequence number */
    tcp_seq th_ack;             /* acknowledgement number */
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int8_t th_x2:4;           /* (unused) */
    u_int8_t th_off:4;          /* data offset */
#  endif
#  if __BYTE_ORDER == __BIG_ENDIAN
    u_int8_t th_off:4;          /* data offset */
    u_int8_t th_x2:4;           /* (unused) */
#  endif
    u_int8_t th_flags;
#  define TH_FIN        0x01
#  define TH_SYN        0x02
#  define TH_RST        0x04
#  define TH_PUSH       0x08
#  define TH_ACK        0x10
#  define TH_URG        0x20
    u_int16_t th_win;           /* window */
    u_int16_t th_sum;           /* checksum */
    u_int16_t th_urp;           /* urgent pointer */
};

struct pdphdr_v1_t {
	uint8_t		pn:1;
	uint8_t		s:1;
	uint8_t		e:1;
	uint8_t		unused:1;	
	uint8_t		pt:1;
	uint8_t		ver:3;
	uint8_t		type;
	uint16_t	len;
	uint32_t	teid;
};

struct pdphdr_v0_t {
	uint8_t		snn:1;
	uint8_t		unused:3;	
	uint8_t		pt:1;
	uint8_t		ver:3;
	uint8_t		type;
	uint16_t	len;
	uint16_t	seq;
	uint16_t	flow_label;
	uint8_t		sndcp_num;
	uint8_t		unused_1;
	uint8_t		unused_2;
	uint8_t		unused_3;
	uint64_t	tid;
};

