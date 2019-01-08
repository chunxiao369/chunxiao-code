#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <getopt.h>
#include <termios.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>


#include "ewx_pcap.h"

#define MAX_LEN 8192

int main(int argc, char **argv)
{
    int fd = 0;
    int file_hdr_len = sizeof(ewx_pcap_hdr_t);
    int len = 0;
    int counter = 0;
    ewx_pcap_hdr_t f_hdr;
    ewx_pcaprec_hdr_t hdr;
    char ptr[MAX_LEN] = {0};
    char *file_name = argv[1];

    if (argc < 2) {
        printf("%s filename\n", argv[0]);
        return 0;
    }
    fd = open(file_name, O_RDONLY,
         S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        return -1;
    }

    if (file_hdr_len != read(fd, &f_hdr, sizeof(ewx_pcap_hdr_t))) {
        printf("read pcap file header error!\n");
        return 1;
    }
    while (1) {
        len = ewx_pcap_read_simple(fd, &hdr, ptr, MAX_LEN);
        printf("incl len is %u.\n", hdr.incl_len);
        if (len > MAX_LEN) {
            printf("incl len is %u.\n", hdr.incl_len);
            printf("Warning, there is packet larges than bs, enlarge bs please!\n");
            continue;
        }
        if (len <= 0) {
            printf("pcap file read end!\n");
            break;
        }
        counter++;
    }
    close(fd);
    printf("counter is %d.\n", counter);
    return 0;
}
