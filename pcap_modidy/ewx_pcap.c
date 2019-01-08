/**
 * @file ewx_pcap.c
 * @Synopsis
 * @author xu.chunxiao@embedway.com
 * @version 0.9.2
 * @date 2013-08-21
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/queue.h>
#define __USE_LARGEFILE64
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>
#include <netinet/in.h>

#include "ewx_pcap.h"

#define EWX_MMAP

#define MAP_SIZE (1ul << 30)    /* MAP_SIZE should >= PAGE_SIZE */

static int64_t ewx_preread_file(ewx_pcap_info_t * pcap)
{
    uint64_t len;
    void *p = NULL;
    int page_size = sysconf(_SC_PAGE_SIZE);
    int delta;
#ifdef EWX_COMMON
    int64_t len2 = 0ll;
    uint64_t tmp = pcap->c_ptr - pcap->ptr;
#endif

#ifdef EWX_MMAP
    /* find offset where align to PAGE_SIZE */
    delta = pcap->offset % page_size;
    pcap->offset -= delta;

    /* calc mmap len */
    //if (pcap->file_len - (pcap->offset + tmp) + delta >= MAP_SIZE) {
    if (pcap->file_len >= MAP_SIZE + pcap->offset) {
        len = MAP_SIZE;
    } else {
        len = pcap->file_len - pcap->offset;
    }

    //printf("preread file offset: %lu, len: %lu.\n", pcap->offset, len);
    p = mmap64(NULL, len, PROT_READ, MAP_PRIVATE, pcap->fd2, pcap->offset);
    if (p == MAP_FAILED) {
        printf("embedway mmap result: %p, error: %d, %s!\n", p, errno, strerror(errno));
        return -1ll;
    }
    pcap->ptr = p;
#endif
#ifdef EWX_COMMON
    if (pcap->file_len >= MAP_SIZE + pcap->offset) {
        len = MAP_SIZE;
    } else {
        len = pcap->file_len - pcap->offset;
    }

    if (pcap->length != 0) {
        len2 = (int64_t) (pcap->length - tmp);
        if (len2 < 0) {
            printf("len2 is error!\n");
        }
        len2 = -len2;
        lseek(pcap->fd2, len2, SEEK_CUR);
        //printf("len2 : %ld\n", len2);
    } else {
        lseek(pcap->fd2, 0, SEEK_SET);
    }
    len2 = read(pcap->fd2, pcap->buff, len);
    if (len2 != len) {
        printf("read error!\n");
        return -1ll;
    }
    pcap->ptr = pcap->buff;
#endif
    pcap->length = len;
    pcap->c_ptr = pcap->ptr + delta;
    return 0ll;
}

static int64_t ewx_free_pcapbuff(ewx_pcap_info_t * pcap)
{
    if (pcap->ptr) {
#ifdef EWX_MMAP
        munmap(pcap->ptr, pcap->length);
#endif
    }
    return 0ll;
}

void ewx_show_pcap(ewx_pcap_info_t * pcap)
{
    printf("pcap->ptr     :void *   %p\n", pcap->ptr);
    printf("pcap->c_ptr   :void *   %p\n", pcap->c_ptr);
    printf("pcap->offset  :uint64_t %lu\n", pcap->offset);
    printf("pcap->length  :uint64_t %lu\n", pcap->length);
    printf("pcap->file_len:uint64_t %lu\n\n", pcap->file_len);
}

ewx_pcap_info_t *ewx_pcap_open(char *filename, uint16_t port)
{
    ewx_pcap_info_t *pcap = NULL;
    uint64_t filesize = -1;
    int64_t ret = 0ll;
    struct stat64 statbuff;
    int page_size = sysconf(_SC_PAGE_SIZE);

    if (filename == NULL) {
        printf("%s: filename is NULL\n", __FUNCTION__);
        goto leave;
    }
    printf("system page size is %d.\n", page_size);
    if (MAP_SIZE < (uint64_t) page_size) {
        printf("map size is too small!\n");
        goto leave;
    }
    if (stat64((const char *)filename, &statbuff) < 0) {
        printf("%s: size get error!\n", filename);
        goto leave;
    } else {
        filesize = statbuff.st_size;
    }

    //pcap = (ewx_pcap_info_t *) rte_malloc("PCAP info", sizeof(ewx_pcap_info_t), CACHE_LINE_SIZE);
    pcap = (ewx_pcap_info_t *) malloc(sizeof(ewx_pcap_info_t));
    if (pcap == NULL) {
        printf("%s: malloc failed for ewx_pcap_info_t structure\n", __FUNCTION__);
        goto leave;
    }
    memset((char *)pcap, 0, sizeof(ewx_pcap_info_t));

    pcap->fd = fopen((const char *)filename, "r");
    if (pcap->fd == NULL) {
        printf("%s: failed for (%s)\n", __FUNCTION__, filename);
        goto leave;
    }

    if (fread(&pcap->info, 1, sizeof(ewx_pcap_hdr_t), pcap->fd) != sizeof(ewx_pcap_hdr_t)) {
        printf("%s: failed to read the file header\n", __FUNCTION__);
        goto leave;
    }
    // Default to little endian format.
    pcap->endian = LITTLE_ENDIAN;
    pcap->filename = strdup(filename);

    // Make sure we have a valid PCAP file for Big or Little Endian formats.
    if ((pcap->info.magic_number != EWX_MAGIC_NUMBER) && (pcap->info.magic_number != ntohl(EWX_MAGIC_NUMBER))) {
        printf("%s: Magic Number does not match!\n", __FUNCTION__);
        fflush(stdout);
        goto leave;
    }
    // Convert from big-endian to little-endian.
    if (pcap->info.magic_number == ntohl(EWX_MAGIC_NUMBER)) {
        printf("PCAP: Big Endian file format found, converting to little endian\n");
        pcap->endian = BIG_ENDIAN;
        pcap->info.magic_number = ntohl(pcap->info.magic_number);
        pcap->info.network = ntohl(pcap->info.network);
        pcap->info.sigfigs = ntohl(pcap->info.sigfigs);
        pcap->info.snaplen = ntohl(pcap->info.snaplen);
        pcap->info.thiszone = ntohl(pcap->info.thiszone);
        pcap->info.version_major = ntohs(pcap->info.version_major);
        pcap->info.version_minor = ntohs(pcap->info.version_minor);
    }

    pcap->fd2 = open(filename, O_RDONLY);
    if (pcap->fd2 < 0) {
        printf("open error!\n");
        goto leave;
    }
    pcap->file_len = filesize;
#ifdef EWX_COMMON
    pcap->buff = rte_zmalloc_socket("PCAP buffer", MAP_SIZE, CACHE_LINE_SIZE, 0);
    //pcap->buff = rte_malloc("PCAP buffer", MAP_SIZE, CACHE_LINE_SIZE);
    //pcap->buff = memalign((size_t)page_size, (size_t)MAP_SIZE);
    if (pcap->buff == NULL) {
        printf("pcap buffer alloc failed.!\n");
        goto leave;
    }
    lseek(pcap->fd2, 0ul, SEEK_SET);
#endif
    ret = ewx_preread_file(pcap);
    if (ret < 0ll) {
        goto leave;
    }
    pcap->c_ptr = pcap->ptr + sizeof(ewx_pcap_hdr_t);

    ewx_pcap_info(pcap, port, 0);

    return pcap;

  leave:
    ewx_pcap_close(pcap);
    fflush(stdout);

    return NULL;
}

void ewx_pcap_info(ewx_pcap_info_t * pcap, uint16_t port, int flag)
{
    printf("\nPCAP file for port %d: %s\n", port, pcap->filename);
    printf("  magic: %08x,", pcap->info.magic_number);
    printf(" Version: %d.%d,", pcap->info.version_major, pcap->info.version_minor);
    printf(" Zone: %d,", pcap->info.thiszone);
    printf(" snaplen: %d,", pcap->info.snaplen);
    printf(" sigfigs: %d,", pcap->info.sigfigs);
    printf(" network: %d", pcap->info.network);
    printf(" Endian: %s\n", pcap->endian == BIG_ENDIAN ? "Big" : "Little");
    if (flag)
        printf("  Packet count: %d\n", pcap->pkt_count);
    printf("\n");
    fflush(stdout);
}

void ewx_pcap_rewind(ewx_pcap_info_t * pcap)
{
    if (pcap == NULL)
        return;

    // Rewind to the beginning
    //rewind(pcap->fd);

    // Seek past the pcap header
    //(void)fseek(pcap->fd, sizeof(ewx_pcap_hdr_t), SEEK_SET);
    if (MAP_SIZE >= pcap->file_len) {
        pcap->offset = 0ull;
        pcap->c_ptr = pcap->ptr + sizeof(ewx_pcap_hdr_t);
        return;
    }
    ewx_free_pcapbuff(pcap);
    pcap->offset = 0ull;
    pcap->length = 0ull;
    pcap->ptr = NULL;
    pcap->c_ptr = NULL;
    ewx_preread_file(pcap);
    pcap->c_ptr = pcap->ptr + sizeof(ewx_pcap_hdr_t);
}

static __inline__ void ewx_pcap_convert(ewx_pcap_info_t * pcap, ewx_pcaprec_hdr_t * pHdr)
{
    if (pcap->endian == BIG_ENDIAN) {
        pHdr->incl_len = ntohl(pHdr->incl_len);
        pHdr->orig_len = ntohl(pHdr->orig_len);
        pHdr->ts_sec = ntohl(pHdr->ts_sec);
        pHdr->ts_usec = ntohl(pHdr->ts_usec);
    }
}

void ewx_pcap_skip(ewx_pcap_info_t * pcap, uint32_t skip)
{
    /* don't support */
    return;
    ewx_pcaprec_hdr_t hdr, *phdr;

    if (pcap == NULL)
        return;

    // Rewind to the beginning
    rewind(pcap->fd);

    // Seek past the pcap header
    (void)fseek(pcap->fd, sizeof(ewx_pcap_hdr_t), SEEK_SET);

    phdr = &hdr;
    while (skip--) {
        if (fread(phdr, 1, sizeof(ewx_pcaprec_hdr_t), pcap->fd) != sizeof(ewx_pcaprec_hdr_t))
            break;

        // Convert the packet header to the correct format.
        ewx_pcap_convert(pcap, phdr);

        (void)fseek(pcap->fd, phdr->incl_len, SEEK_CUR);
    }
}

void ewx_pcap_close(ewx_pcap_info_t * pcap)
{
    if (pcap == NULL)
        return;

    if (pcap->fd)
        fclose(pcap->fd);
    if (pcap->filename)
        free(pcap->filename);
    ewx_free_pcapbuff(pcap);
    free(pcap);
}

static int ewx_pcap_read_prepare(ewx_pcap_info_t * pcap, int len)
{
    int64_t ret = 0ll;
    uint64_t tmp = pcap->c_ptr - pcap->ptr;
    if (pcap->length - tmp >= (uint64_t) len) {
        return 0;
    } else if (pcap->file_len <= (pcap->offset + tmp + len)) {
        return -1;
    } else {
        /* move ahead */
        ewx_free_pcapbuff(pcap);
        pcap->offset += tmp;
        ret = ewx_preread_file(pcap);
        if (ret < 0ll) {
            return -1;
        }
        return 0;
    }
}

size_t ewx_pcap_read(ewx_pcap_info_t * pcap, ewx_pcaprec_hdr_t * pHdr, char *pktBuff, uint32_t bufLen)
{
    int len;
    len = sizeof(ewx_pcaprec_hdr_t);

    do {
        //if (fread(pHdr, 1, len, pcap->fd) != len)
        //    return 0;
        if (ewx_pcap_read_prepare(pcap, len)) {
            return 0;
        } else {
            memcpy((void *)pHdr, pcap->c_ptr, len);
            pcap->c_ptr += len;
        }

        // Convert the packet header to the correct format.
        ewx_pcap_convert(pcap, pHdr);

        // Skip packets larger then the buffer size.
        if (pHdr->incl_len > bufLen) {
            if (ewx_pcap_read_prepare(pcap, pHdr->incl_len)) {
                return 0;
            } else {
                pcap->c_ptr += pHdr->incl_len;
            }
            //(void)fseek(pcap->fd, pHdr->incl_len, SEEK_CUR);
            return pHdr->incl_len;
        }
        //return fread(pktBuff, 1, pHdr->incl_len, pcap->fd);
        if (ewx_pcap_read_prepare(pcap, pHdr->incl_len)) {
            return 0;
        } else {
            memcpy(pktBuff, pcap->c_ptr, pHdr->incl_len);
            pcap->c_ptr += pHdr->incl_len;
        }
        return pHdr->incl_len;
    } while (1);
}

size_t ewx_pcap_read2(ewx_pcap_info_t * pcap, ewx_pcaprec_hdr_t * pHdr, char **pktBuff, uint32_t bufLen)
{
    int len;
    len = sizeof(ewx_pcaprec_hdr_t);

    do {
        //if (fread(pHdr, 1, len, pcap->fd) != len)
        //    return 0;
        if (ewx_pcap_read_prepare(pcap, len)) {
            return 0;
        } else {
            memcpy((void *)pHdr, pcap->c_ptr, len);
            pcap->c_ptr += len;
        }

        // Convert the packet header to the correct format.
        ewx_pcap_convert(pcap, pHdr);

        // Skip packets larger then the buffer size.
        if (pHdr->incl_len > bufLen) {
            if (ewx_pcap_read_prepare(pcap, pHdr->incl_len)) {
                return 0;
            } else {
                pcap->c_ptr += pHdr->incl_len;
            }
            //(void)fseek(pcap->fd, pHdr->incl_len, SEEK_CUR);
            return pHdr->incl_len;
        }
        //return fread(pktBuff, 1, pHdr->incl_len, pcap->fd);
        if (ewx_pcap_read_prepare(pcap, pHdr->incl_len)) {
            return 0;
        } else {
            *pktBuff = (char *)pcap->c_ptr;
            pcap->c_ptr += pHdr->incl_len;
        }
        return pHdr->incl_len;
    } while (1);
}

int ewx_pcap_write_file_header(int fd, ewx_pcap_hdr_t * hdr)
{
    if (write(fd, hdr, sizeof(ewx_pcap_hdr_t)) < 0) {
        printf("write file header error!\n");
        return -1;
    }
    return 0;
}

int ewx_pcap_set_header(ewx_pcaprec_hdr_t * hdr, int len)
{
    struct timeval tm;

    gettimeofday(&tm, NULL);
    hdr->ts_sec = tm.tv_sec;
    hdr->ts_usec = tm.tv_usec;
    hdr->incl_len = len;
    hdr->orig_len = len;

    return 0;
}

size_t ewx_pcap_read_simple(int fd, ewx_pcaprec_hdr_t * pHdr, char *pktBuff, uint32_t bufLen)
{
    int len;
    len = sizeof(ewx_pcaprec_hdr_t);

    do {
        if (read(fd, pHdr, len) != len)
            return 0;

        // Skip packets larger then the buffer size.
        if (pHdr->incl_len > bufLen) {
            lseek(fd, pHdr->incl_len, SEEK_CUR);
            return pHdr->incl_len;
        }

        return read(fd, pktBuff, pHdr->incl_len);
    } while (1);
}
