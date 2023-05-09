#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <gcrypt.h>
#include "iquic.h"
static int quic_version(uint8_t * payload, uint16_t plen, uint32_t * get_version)
{
    uint32_t version;
    u_int8_t first_byte;
    u_int8_t pub_bit1, pub_bit2, pub_bit3, pub_bit4, pub_bit5, pub_bit7, pub_bit8;

    /* According to draft-ietf-quic-transport-29: "Clients MUST ensure that UDP
       datagrams containing Initial packets have UDP payloads of at least 1200
       bytes". Similar limit exists for previous versions */
    if (plen < 1200) {
        return 0;
    }
    first_byte = payload[0];
    pub_bit1 = ((first_byte & 0x80) != 0);
    pub_bit2 = ((first_byte & 0x40) != 0);
    pub_bit3 = ((first_byte & 0x20) != 0);
    pub_bit4 = ((first_byte & 0x10) != 0);
    pub_bit5 = ((first_byte & 0x08) != 0);
    pub_bit7 = ((first_byte & 0x02) != 0);
    pub_bit8 = ((first_byte & 0x01) != 0);

    version = 0;
    if (pub_bit1) {
        version = ntohl(*((u_int32_t *) & payload[1]));
    } else if (pub_bit5 && !pub_bit2) {
        if (!pub_bit8) {
            printf("Packet without version\n");
        } else {
            version = ntohl(*((u_int32_t *) & payload[9]));
        }
    }
    if (!is_version_valid(version)) {
        printf("Invalid version 0x%x\n", version);
        return 0;
    }

    if (is_gquic_ver_less_than(version, 43) && (!pub_bit5 || pub_bit3 != 0 || pub_bit4 != 0)) {
#ifdef QUIC_DEBUG
        printf("Version 0x%x invalid flags 0x%x\n", version, first_byte);
#endif
        return 0;
    }
    if ((version == V_Q046) && (pub_bit7 != 1 || pub_bit8 != 1)) {
#ifdef QUIC_DEBUG
        printf("Q46 invalid flag 0x%x\n", first_byte);
#endif
        return 0;
    }
    if ((is_version_quic(version) || (version == V_Q046) || (version == V_Q050)) && (pub_bit3 != 0 || pub_bit4 != 0)) {
        printf("Version 0x%x not Initial Packet\n", version);
        return 0;
    }
    *get_version = version;
    return 1;
}

static uint8_t *decrypt_initial_packet(uint8_t * payload, uint16_t plen,
                                       const uint8_t * dest_conn_id, uint8_t dest_conn_id_len,
                                       uint8_t source_conn_id_len, uint32_t version, uint16_t *clear_payload_len)
{
    uint64_t token_length, payload_length, packet_number;
    uint8_t first_byte;
    uint32_t pkn32, pn_offset, pkn_len, offset;
    quic_cipher cipher = { 0 }; /* Client initial cipher */
    quic_decrypt_result_t decryption = { 0 };
    uint8_t client_secret[HASH_SHA2_256_LENGTH];

    if (quic_derive_initial_secrets(version, dest_conn_id, dest_conn_id_len, client_secret) != 0) {
        printf("Error quic_derive_initial_secrets\n");
        return NULL;
    }

    /* Packet numbers are protected with AES128-CTR,
       Initial packets are protected with AEAD_AES_128_GCM. */
    if (!quic_cipher_prepare(&cipher, GCRY_MD_SHA256, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_GCM, client_secret)) {
        printf("Error quic_cipher_prepare\n");
        return NULL;
    }

    /* Type(1) + version(4) + DCIL + DCID + SCIL + SCID */
    pn_offset = 1 + 4 + 1 + dest_conn_id_len + 1 + source_conn_id_len;
    pn_offset += quic_len(&payload[pn_offset], &token_length);
    pn_offset += token_length;
    /* Checks: quic_len reads 8 bytes, at most; quic_decrypt_header reads other 20 bytes */
    if (pn_offset + 8 + (4 + 16) >= plen)
        return NULL;
    pn_offset += quic_len(&payload[pn_offset], &payload_length);

    printf("pn_offset %d token_length %lu payload_length %lu\n", pn_offset, token_length, payload_length);

    if (!quic_decrypt_header(&payload[0], pn_offset, cipher.hp_cipher, GCRY_CIPHER_AES128, &first_byte, &pkn32)) {
        quic_cipher_reset(&cipher);
        return NULL;
    }
    printf("first_byte 0x%x pkn32 0x%x\n", first_byte, pkn32);

    pkn_len = (first_byte & 3) + 1;
    /* TODO: is it always true in Initial Packets? */
    packet_number = pkn32;

    offset = pn_offset + pkn_len;
    quic_decrypt_message(&cipher, &payload[0], plen,
                         offset, first_byte, pkn_len, packet_number, &decryption);

    quic_cipher_reset(&cipher);
    if (decryption.data_len) {
        *clear_payload_len = (uint16_t)decryption.data_len;
        return decryption.data;
    }

    return NULL;
}

static int quic_decap(uint8_t * payload, uint16_t plen)
{
    int isq;
    u_int8_t *clear_payload;
    uint16_t clear_payload_len;
    uint32_t get_version;
    u_int8_t dest_conn_id_len, source_conn_id_len;
    isq = quic_version(payload, plen, &get_version);
    if (!isq) {
        return 1;
    }
    if (!is_version_supported(get_version)) {
        return 1;
    }

    if (is_gquic_ver_less_than(get_version, 43)) {
        clear_payload = (uint8_t *) & payload[26];
        clear_payload_len = plen - 26;
        /* Skip Private-flag field for version for < Q34 */
        if (is_gquic_ver_less_than(get_version, 33)) {
            clear_payload += 1;
            (clear_payload_len) -= 1;
        }
    } else if (get_version == V_Q046) {
        if (payload[5] != 0x50) {
            printf("Q46 invalid conn id len 0x%x\n", payload[5]);
            return -1;
        }
        clear_payload = (uint8_t *) & payload[30];
        clear_payload_len = plen - 30;
    } else {
        dest_conn_id_len = payload[5];
        if (dest_conn_id_len == 0 || dest_conn_id_len > QUIC_MAX_CID_LENGTH) {
            printf("Packet 0x%x with dest_conn_id_len %d\n", get_version, dest_conn_id_len);
            return -1;
        }
        source_conn_id_len = payload[6 + dest_conn_id_len];
        if (source_conn_id_len > QUIC_MAX_CID_LENGTH) {
            printf("Packet 0x%x with source_conn_id_len %d\n", get_version, source_conn_id_len);
            return -1;
        }
        const u_int8_t *dest_conn_id = &payload[6];
        clear_payload = decrypt_initial_packet(payload, plen,
                                               dest_conn_id, dest_conn_id_len,
                                               source_conn_id_len, get_version, &clear_payload_len);
        printf("payloadlen after decry: %u\n", clear_payload_len);
        /*
         TODO
         servername should decap by tls
         */
        printf("servername after decry: %s\n", clear_payload + 64);
    }

    return 0;
}

static void my_packet_handler(u_char * args, const struct pcap_pkthdr *header, const u_char * packet)
{
    /* First, lets make sure we have an IP packet */
    struct ether_header *eth_header;
    eth_header = (struct ether_header *)packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("Not an IP packet. Skipping...\n\n");
        return;
    }

    /* The total packet length, including all headers
       and the data payload is stored in
       header->len and header->caplen. Caplen is
       the amount actually available, and len is the
       total packet length even if it is larger
       than what we currently have captured. If the snapshot
       length set with pcap_open_live() is too small, you may
       not have the whole packet. */
    printf("Total packet available: %d bytes\n", header->caplen);
    printf("Expected packet size: %d bytes\n", header->len);

    /* Pointers to start point of various headers */
    const u_char *ip_header;
    const u_char *udp_header;
    const u_char *payload;

    /* Header lengths in bytes */
    int ethernet_header_length = 14;    /* Doesn't change */
    int ip_header_length;
    int udp_header_length;
    int payload_length;

    /* Find start of IP header */
    ip_header = packet + ethernet_header_length;
    /* The second-half of the first byte in ip_header
       contains the IP header length (IHL). */
    ip_header_length = ((*ip_header) & 0x0F);
    /* The IHL is number of 32-bit segments. Multiply
       by four to get a byte count for pointer arithmetic */
    ip_header_length = ip_header_length * 4;
    printf("IP header length (IHL) in bytes: %d\n", ip_header_length);

    /* Now that we know where the IP header is, we can
       inspect the IP header for a protocol number to
       make sure it is TCP before going any further.
       Protocol is always the 10th byte of the IP header */
    u_char protocol = *(ip_header + 9);
    if (protocol != IPPROTO_UDP) {
        printf("Not a UDP packet. Skipping...\n\n");
        return;
    }

    /* Add the ethernet and ip header length to the start of the packet
       to find the beginning of the UDP header */
    udp_header = packet + ethernet_header_length + ip_header_length;
    printf("sport: %u, dport: %u\n", ntohs(*(uint16_t *) udp_header), ntohs(*(uint16_t *) (udp_header + 2)));
    udp_header_length = 8;
    printf("UDP header length in bytes: %d\n", udp_header_length);

    /* Add up all the header sizes to find the payload offset */
    int total_headers_size = ethernet_header_length + ip_header_length + udp_header_length;
    printf("Size of all headers combined: %d bytes\n", total_headers_size);
    payload_length = header->caplen - (ethernet_header_length + ip_header_length + udp_header_length);
    printf("Payload size: %d bytes\n", payload_length);
    payload = packet + total_headers_size;
    printf("Memory address where payload begins: %p\n\n", payload);

    quic_decap((uint8_t *) payload, payload_length);
    /* Print payload in ASCII */
    /*
       if (payload_length > 0) {
       const u_char *temp_pointer = payload;
       int byte_count = 0;
       while (byte_count++ < payload_length) {
       printf("%c", *temp_pointer);
       temp_pointer++;
       }
       printf("\n");
       }
     */

    return;
}

int main(int argc, char **argv)
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    int total_packet_count = 5;
    pcap_t *handle;
    /* End the loop after this many packets are captured */
    u_char *my_arguments = NULL;

    handle = pcap_open_offline("iquic.pcap", error_buffer);
    pcap_loop(handle, total_packet_count, my_packet_handler, my_arguments);

    return 0;
}
