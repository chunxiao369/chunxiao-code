
#define PA_FPE_L7_DNS_URL_MAX_LENGTH      260
#define PA_FPE_L7_DNS_ENTRY_NUM           21000
#define PA_FPE_L7_DNS_BUCKET_DEEP         13
#define PA_FPE_L7_DNS_BUCKETS_NUM         PA_FPE_L7_DNS_ENTRY_NUM * 2
#define PA_FPE_L7_DNS_URL_RELATED_IP_NUM  20
#define PA_FPE_L7_DNS_IP_LISTS_NUM        PA_FPE_L7_DNS_ENTRY_NUM 
#define PA_FPE_L7_DNS_ERROR_INIT          0x0101


typedef struct pa_fpe_parser_dns_hdr_s {
    uint16_t ID;
    uint16_t QR : 1;
    uint16_t opcode : 4;
    uint16_t AA : 1;
    uint16_t TC : 1;
    uint16_t RD : 1;
    uint16_t RA : 1;
    uint16_t zero : 3;
    uint16_t rcode : 4;
    uint16_t Questions;
    uint16_t AnswerRRs;
    uint16_t AuthorityRRs; 
    uint16_t AdditionalRRs;
} pa_fpe_parser_dns_hdr_t;

#define __DNSDEBUG
#ifdef __DNSDEBUG
#define dnsdebug(fmt, args...)                               \
do {                                                         \
    printf(fmt, ##args);                                     \
} while(0)
#else
#define dnsdebug(fmt, args...)
#endif

int process_dns(uint8_t *packet, uint32_t length);
