/**
 * @file testpcap.c
 * @Brief libpcap example
 * @author xu.chunxiao@embedway.com
 * @version 1.0
 * @date 2021-08-02
 */
#include <time.h>
#include <pcap.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#if 0
struct pcap_pkthdr {
    struct timeval ts;  /* time stamp */
    bpf_u_int32 caplen; /* length of portion present */
    bpf_u_int32 len;    /* length this packet (off wire) */
};
#endif

#define DSIZE 512
int main(int argc, char **argv)
{
    uint16_t ruleid = 6;
    uint32_t entryid = 7;
    //pcap_t *pcap_open_dead(int linktype, int snaplen);
    uint8_t data[DSIZE] = {0};
    int i = 0;
    pcap_t *dev = NULL;
    pcap_dumper_t *dumper = NULL;
    struct pcap_pkthdr header;
    struct timeval tm;

    char name[512] = {0};
    char str_time[16] = {0};
    time_t time;

    //dir and name
    gettimeofday(&tm, NULL);
    time = tm.tv_sec;
    strftime(str_time, 16, "%Y-%m-%d", localtime(&time));
    sprintf(name, "/packet_dump/rule_%02d/%s/%d/", ruleid, str_time, entryid);
    printf("line: %d, name: %s\n", __LINE__, name);
    mkdir(name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    strcpy(name + strlen(name), "xxx.pcap");
    printf("line: %d, name: %s\n", __LINE__, name);

    dev = pcap_open_dead(DLT_EN10MB, 65535);
    if (NULL == dev) {
        printf("line: %d.\n", __LINE__);
        return 1;
    }
    if (access(name, F_OK) != 0) {
        dumper = pcap_dump_open(dev, name);
    } else {
        dumper = pcap_dump_open_append(dev, name);
    }
    if (NULL == dumper) {
        printf("line: %d.\n", __LINE__);
        return 1;
    }
    for (i = 0; i < DSIZE; i++) {
        data[i] = 'x';
    }

    gettimeofday(&tm, NULL);
    header.ts = tm;
    header.caplen = DSIZE;
    header.len = DSIZE;
    pcap_dump((u_char *)dumper, &header, data);
    pcap_dump_flush(dumper);
    pcap_dump_close(dumper);

    return 0;
}
