#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <signal.h>
#include <netinet/if_ether.h>
#include "packet.h"

int  breakup = 0;  
void (*original_sig_int)(int num);
void (*original_sig_term)(int num);

void usage()
{
}

void cap_term(int signum)
{
	breakup = 1;
	if (original_sig_int)
		signal(SIGINT, original_sig_int);
	if(original_sig_term)
		signal(SIGTERM, original_sig_term);
}

uint64_t process(pcap_t *handle)
{
    struct          pcap_pkthdr header;
    uint64_t        counter = 0ull;
    const u_char    *packet;
    uint32_t        packet_length = 0;
    do {
        packet = pcap_next(handle, &header);

        if (packet != NULL ) {
            counter++;
            packet_length = header.caplen;
            //printf("packet_length: %d.\n", header.caplen);
            if (process_dns((uint8_t *)packet, packet_length)) {
                //printf("dns response success.\n");
            } else {
                //printf("dns not response.\n");
            } 
        } else {
            break;
        }
    } while (1);
    return counter;
}

int main(int argc, char *argv[])
{
    int     result;
    int     len;
	int 	i_flag  = 0;
    int     f_flag  = 0;
    int     o_flag  = 0;
    int     t_flag  = 0;
	pcap_t  *handle = NULL;
    char    errbuf[PCAP_ERRBUF_SIZE];
	char 	name[128];
    while((result = getopt(argc, argv, "i:f:o:t:")) != -1) {
		switch (result) {
		case 'i':
			i_flag = 1;
			if (f_flag == 0) {
				handle = pcap_open_live(optarg, 4096, 1, 3000, errbuf);
			} else {
				printf("ERROR: interface and file argment couldnot enable at the same time\n");
				exit (-1);
			}
			break;
		case 'f':
			f_flag = 1;
			if (i_flag == 0) {
				handle = pcap_open_offline(optarg, errbuf);
			} else {
				printf("ERROR: interface and file argment couldnot enable at the same time\n");
				exit (-1);
			}			
			break;
		case 'o':
			o_flag = 1;
			len = strlen(optarg);
			if (len > 128) {
				printf("output file name too long!\n");
				exit (-1);
			}
			memset(name, '\0', sizeof(name));
			strncpy(name, optarg, len);				
			break;
		case 't':
			if (strncasecmp(optarg, "gi", 2) == 0) {
				t_flag = 1;
			} else if (strncasecmp(optarg, "gn", 2) == 0) {
				t_flag = 0;
			} else {
				printf("argument 't' only support 'gn' or 'gi'\n");
				exit (-1);
			}
			break;
		default:
			usage();
			break;
		}
	}
    original_sig_int = signal(SIGINT, cap_term);
    original_sig_term = signal(SIGTERM, cap_term);

    if (handle == NULL) {
        printf("error");
    }
    if (f_flag == 1) {
        printf("counter: %llu\n", process(handle));        
    }
    return 0;
}


