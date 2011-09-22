#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "dns.h"

static int url_convert (uint8_t * ptr, uint16_t offset,\
                           uint16_t *len, uint8_t *packet_tail,\
                           char *query_name)
{
    dnsdebug (" IN@url_convert ptr_value16 = %04x offset = %d\n", *(uint16_t*)ptr, offset);
    uint8_t *result, *ptr_ = ptr + offset;
    int notCompress = 1;
    int counter = 0;

    *len = 0;
    result = (uint8_t *)malloc(PA_FPE_L7_DNS_URL_MAX_LENGTH);
    if (result == NULL) {
        printf("Error : cannot alloc memory for url_mem\n");
        dnsdebug("OUT@url_convert, cannot alloc memory for url_mem\n");
        return -1;
    }
    int i;
    for (i = 0; i < packet_tail - ptr_; i++) {
        if (i % 16 == 0)
            printf("\n");
        printf("%02x ", ptr_[i]);
    }
    printf("\n");
  
    while ((*ptr_ != 0) && (ptr_ <= packet_tail)) {
        counter++;
        dnsdebug ("   @url_convert, seglen = %2x\n", *ptr_);
        if (*ptr_ < 0xC0) {
            *len += (notCompress ? (*ptr_) + 1 : 0);
            if (((result + *ptr_ + 1) > (result + PA_FPE_L7_DNS_URL_MAX_LENGTH))) {
                goto error;
            }
            memcpy(result, ptr_ + 1, *ptr_);
            result += *ptr_;
            ptr_ += (*ptr_ + 1);
            if (*ptr_ != 0) {
                *(result++) = '.';
            } else {
                *len += (notCompress ? 1 : 0);
                *result = 0;
            }
        } else {
            *len += (notCompress ? 2 : 0);
            notCompress = 0;
            if ((ptr_ == (ptr + (*(uint16_t *)ptr_ - 0xc000))) || *(uint16_t *)ptr_ <= 0xc012) {
                /*线上发现有些错包，在dns request和dns response中的压缩模式偏移会指向自己，引起死循环*/
                goto error;
            } else {
                ptr_ = ptr + (*(uint16_t *)ptr_ - 0xC000);
            }
        }

        if (counter >= 100) {
            /*仅仅是调试，调试通过需要删除*/
            printf("[3], counter %d, *len=%d, *ptr_=%x, ptr_=%p, ptr=%p, notCompress=%d\n",
                   counter, *len, *(uint16_t *)ptr_, ptr_, ptr, notCompress);
            *result = '\0';
            goto error;
            //return (char *)2;
        }
    }
    printf("come ....................\n");
    if (ptr_ > packet_tail) {
    printf("come ...................., %d\n", __LINE__);
        goto error;
    }

    printf("come ...................., %d\n", __LINE__);
    memcpy(query_name, result, PA_FPE_L7_DNS_URL_MAX_LENGTH);
    printf("come ...................., %d\n", __LINE__);
    printf("query_name: %s.\n", query_name);
    printf("result: %s.\n", result);
    free((void *)result);
    dnsdebug ("OUT@url_convert len = %d, result = %s\n", *len, result);
    //printf("OUT@url_convert len = %d, result = %s\n", *len, result);
    return 0;

error:
    *len = 0;
    free(result);
    return -1;
}

static pa_fpe_parser_dns_hdr_t * is_dns_response(uint8_t *packet)
{
    dnsdebug(" IN@is_dns_response, dst mac = %02x:%02x:%02x:%02x:%02x:%02x\n",\
              *packet, *(packet+1), *(packet+2), *(packet+3), *(packet+4), *(packet+5) );
    int offset = 12;
    while (*(uint16_t *)(packet + offset) == htons(0x8100)) {
        offset += 4;
    }
    if (*(uint16_t *)(packet + offset) != htons(0x0800)) {
        dnsdebug ("OUT@is_dns_response, not IP packet, ethernet.type = %04x\n", *(uint16_t *)(packet+offset));
        return NULL;
    }
    offset += (2 + 9);
    if (*(packet + offset) != 0x11) {
        dnsdebug ("OUT@is_dns_response, not UDP packet, ip.protocol = %04x\n", *(packet + offset));
        return NULL;
    }
    offset += 11;
    if (*(uint16_t *)(packet + offset) != htons(0x35)) {
        dnsdebug ("OUT@is_dns_response, not DNS response, udp.srcport = %04x\n", *(uint16_t *)(packet + offset));
        return NULL;
    }
    offset += 8;
    dnsdebug("OUT@is_dns_response, packet is a dns response\n");
    return (pa_fpe_parser_dns_hdr_t *)(packet + offset);
}

int process_dns(uint8_t *packet, uint32_t length)
{
    pa_fpe_parser_dns_hdr_t *dns_hdr;
    uint8_t *ptr;
    uint16_t len = 0;
    uint8_t *packet_tail;
    char query_name[PA_FPE_L7_DNS_URL_MAX_LENGTH] = {0};
    char *answer_name;
    int offset;
    int i;
    uint16_t answer_type;
    uint32_t ip;
    packet_tail = packet + length;
 
    dns_hdr = (pa_fpe_parser_dns_hdr_t *)is_dns_response(packet);
    if (dns_hdr == NULL) {
        return 1;
    }
    if (dns_hdr->QR == 0 || dns_hdr->AnswerRRs == 0)
        return 1;
    offset = 12;
    ptr = (uint8_t *)(dns_hdr);

    if (url_convert(ptr, offset, &len, packet_tail, query_name)) {
        dnsdebug("OUT@extract_dns, error\n");
        return -1;
    }
    answer_name = query_name;//一般情况下第一个Answer的Name和Query是一样的，因此不再重新解析
    offset += (len + 4);//( len + 2 + 2)
    dnsdebug("   @extract_dns, offset = %d:%2x\n", offset, *(ptr + offset));
    for (i = 0; i < dns_hdr->AnswerRRs; i++) {
        offset += 2;
        answer_type = *(uint16_t *)(ptr + offset);
        dnsdebug("   @extract_dns, answer_type = %d\n", answer_type);
        if (answer_type == 5) {
            offset += 10;
            if (url_convert(ptr, offset, &len, packet_tail, query_name)) {
                dnsdebug("OUT@extract_dns, error\n");
                return -1;
            }
            offset += len;
        } else if (answer_type == 15) {
            /*it's a mx rss*/
            offset += 12;
            if (url_convert(ptr, offset, &len, packet_tail, query_name)) {
                dnsdebug("OUT@extract_dns, error\n");
                return -1;
            }
            offset += len;
            /*add here*/
            /*if (pa_fpe_l7_dns_bucket_search(query_name, &bucket, &bucket_id, &index) == 1 ) {
                pa_fpe_l7_dns_bucket_insert(answer_name, bucket->entry[index]->rule_id);
            }*/

            continue;
        } else if (answer_type == 1) {
            offset += 10;
            ip = *(uint32_t*)(ptr + offset);
            offset += 4;
        }
        if (i == 0) {
#if 0
            if (pa_fpe_l7_dns_bucket_search(query_name, /*answer_name, */&bucket, &bucket_id, &index) != 1 ) {
                return 0;
            }
#endif
        }
        if (answer_type == 1) {
#if 0
            //printf("query_name=%s, ip=0x%x\n", query_name, ip);
            pa_fpe_l7_dns_bucket_update(bucket, bucket_id, index, ip);
            pa_fpe_l4_list_insert(bucket->entry[index]->rule_id, ip, 0, 0,\
                                  L4_IP_TABLE_SEARCH, NULL, &status, NULL, NULL, NULL);
#endif
        }
    }

    return 0;
}

