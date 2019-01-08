
#ifndef _EWX_PCAP_H_
#define _EWX_PCAP_H_

#define EWX_MAGIC_NUMBER	0xa1b2c3d4
#define EwX_MAJOR_VERSION	2
#define EwX_MINOR_VERSION	4
#define CACHE_LINE_SIZE         128
typedef struct ewx_pcap_hdr_s {
    uint32_t magic_number;       /**< magic number */
    uint16_t version_major;      /**< major version number */
    uint16_t version_minor;      /**< minor version number */
    int32_t thiszone;            /**< GMT to local correction */
    uint32_t sigfigs;            /**< accuracy of timestamps */
    uint32_t snaplen;            /**< max length of captured packets, in octets */
    uint32_t network;            /**< data link type */
} ewx_pcap_hdr_t;

typedef struct ewx_pcaprec_hdr_s {
    uint32_t ts_sec;             /**< timestamp seconds */
    uint32_t ts_usec;            /**< timestamp microseconds */
    uint32_t incl_len;           /**< number of octets of packet saved in file */
    uint32_t orig_len;           /**< actual length of packet */
} ewx_pcaprec_hdr_t;

typedef struct ewx_pcap_info_s {
    FILE *fd;                   /**< File descriptor */
    char *filename;             /**< allocated string for filename of pcap */
    uint32_t endian;            /**< Endian flag value */
    uint32_t pkt_size;          /**< Average packet size */
    uint32_t pkt_count;         /**< pcap count of packets */
    uint32_t pkt_idx;           /**< Index into the current PCAP file */
    uint8_t *buff;
    uint8_t *ptr;
    uint8_t *c_ptr;
    uint64_t offset;            /* offset  must be a multiple of */
    /* the page size as returned by sysconf(_SC_PAGE_SIZE) */
    uint64_t length;
    uint64_t file_len;
    int fd2;
    int reserve;
    ewx_pcap_hdr_t info;            /**< information on the PCAP file */
} ewx_pcap_info_t;

#ifndef EwX_BIG_ENDIAN
#define EwX_BIG_ENDIAN		0x4321
#endif
#ifndef EwX_LITTLE_ENDIAN
#define EwX_LITTLE_ENDIAN	0x1234
#endif

typedef struct ewx_pcap_pkt_data_s {        /**< Keep these in this order as pkt_seq_t mirrors the first three objects */
    uint8_t *buffAddr;                  /**< Start of buffer virtual address */
    uint8_t *virtualAddr;               /**< Pointer to the start of the packet data */
    //phys_addr_t           physAddr;       /**< Packet physical address */
    void *physAddr;                     /**< Packet physical address */
    uint32_t size;                      /**< Real packet size (hdr.incl_len) */
    ewx_pcaprec_hdr_t hdr;                  /**< Packet header from the .pcap file for each packet */
} ewx_pcap_pkt_data_t;

extern ewx_pcap_info_t *ewx_pcap_open(char *filename, uint16_t port);
extern int ewx_pcap_valid(char *filename);
extern void ewx_show_pcap(ewx_pcap_info_t * pcap);
extern void ewx_pcap_close(ewx_pcap_info_t * pcap);
extern void ewx_pcap_rewind(ewx_pcap_info_t * pcap);
extern void ewx_pcap_skip(ewx_pcap_info_t * pcap, uint32_t skip);
extern void ewx_pcap_info(ewx_pcap_info_t * pcap, uint16_t port, int flag);
extern size_t ewx_pcap_read(ewx_pcap_info_t * pcap, ewx_pcaprec_hdr_t * pHdr, char *pktBuff, uint32_t bufLen);
extern size_t ewx_pcap_read2(ewx_pcap_info_t * pcap, ewx_pcaprec_hdr_t * pHdr, char **pktBuff, uint32_t bufLen);
extern int ewx_pcap_write_file_header(int fd, ewx_pcap_hdr_t * hdr);
extern int ewx_pcap_set_header(ewx_pcaprec_hdr_t * hdr, int len);
extern size_t ewx_pcap_read_simple(int fd, ewx_pcaprec_hdr_t * pHdr, char *pktBuff, uint32_t bufLen);

#endif /* _WR_PCAP_H_ */
