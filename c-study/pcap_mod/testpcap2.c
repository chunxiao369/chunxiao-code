/**********************************************************************
* file:   testpcap2.c
* date:   2001-Mar-14 12:14:19 AM
* Author: Martin Casado
* Last Modified:2001-Mar-14 12:14:11 AM
*
* Description: Q&D proggy to demonstrate the use of pcap_loop
*
**********************************************************************/

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

FILE *out_file;
char link_lay[14] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,\
                     0x00, 0x01, 0x02, 0x03, 0x04, 0x06, \
                     0x08, 0x00};
/* callback function that is passed to pcap_loop(..) and called each time
 * a packet is recieved                                                    */
struct self_hdr {
    uint32_t sec;
    uint32_t usec;
    uint32_t len;
    uint32_t caplen;
};
void my_callback(u_char * useless, const struct pcap_pkthdr *pkthdr, const u_char * packet)
{
    static int count = 1;
    /* write pcap packet header */
    struct self_hdr hdr;
    hdr.sec  = pkthdr->ts.tv_sec;
    hdr.usec = pkthdr->ts.tv_usec;
    hdr.len        = pkthdr->len + 14;
    hdr.caplen     = pkthdr->caplen + 14;
    fwrite(&hdr, 1, sizeof(struct self_hdr), out_file);
    fwrite(link_lay, 1, 14, out_file);
    fwrite(packet, 1, pkthdr->len, out_file);
    fprintf(stdout, "packet len: %u, count: %d\n", pkthdr->len, count);
    fflush(stdout);
    count++;
}

int main(int argc, char **argv)
{
    int i;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *descr;
    const u_char *packet;
    struct pcap_pkthdr hdr;     /* pcap.h */
    struct ether_header *eptr;  /* net/ethernet.h */
    FILE *in_file;
    struct pcap_file_header f_hdr;

    if (argc != 3) {
        fprintf(stdout, "Usage: %s pcap_file new_file\n", argv[0]);
        return 0;
    }
    if ((in_file = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        return 3;
    }
    if ((out_file = fopen(argv[2], "wb")) == NULL) {
        perror(argv[2]);
        return 3;
    }

    /* write file header */
    fread(&f_hdr, 1, sizeof(struct pcap_file_header), in_file);
    f_hdr.linktype = 1;
    fwrite(&f_hdr, 1, sizeof(struct pcap_file_header), out_file);
    fclose(in_file);

    /* open device for reading */
    descr = pcap_open_offline(argv[1], errbuf);
    if (descr == NULL) {
        printf("pcap_open_offline(): %s\n", errbuf);
        exit(1);
    }
    /* allright here we call pcap_loop(..) and pass in our callback function */
    /* int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user) */
    /* If you are wondering what the user argument is all about, so am I!!   */
    pcap_loop(descr, atoi(argv[1]), my_callback, NULL);

    fprintf(stdout, "\nDone processing packets... wheew!\n");
    fclose(out_file);
    return 0;
}
