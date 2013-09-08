#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#define __USE_LARGEFILE64
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define BLOCK_LEN   (1ull << 20)
#define WR_OK    0
#define WR_ERROR 1
int wr_show_version = 0;
int wr_show_help    = 0;
#define WR_COMMON_IO        1
#define WR_FILE_IO          2
#define WR_MMAP_IO          3
#define WR_LIBAIO_IO        4
int wr_type             = WR_FILE_IO;
uint64_t file_size      = 0ull;
int      block_size     = BLOCK_LEN;
const char *input_file  = NULL;
const char *output_file = NULL;

typedef struct _file_info {
    FILE *c_fd;
    int fd;
    char str[256];
} file_info_t;

#define WR_READ  0x1
#define WR_WRITE 0x2
static int wr_open(file_info_t *f_i, uint32_t attr)
{
    if (wr_type == WR_COMMON_IO) {
        f_i->fd = open(f_i->str, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    } else if (wr_type == WR_FILE_IO) {
        if (attr & WR_READ) {
            f_i->c_fd = fopen(f_i->str, "rb");
        } else if (attr & WR_WRITE) {
            f_i->c_fd = fopen(f_i->str, "wb");
        }
    } else if (wr_type == WR_MMAP_IO) {
    }
    return 0;
}

static int wr_close(file_info_t *f_i)
{
    if (wr_type == WR_COMMON_IO) {
        close(f_i->fd);
    } else if (wr_type == WR_FILE_IO) {
        fclose(f_i->c_fd);
    } else if (wr_type == WR_MMAP_IO) {
    }
    return 0;
}

static int wr_read(char *buff, int block_size, file_info_t *f_i)
{
    int wr_length = 0;
    if (wr_type == WR_COMMON_IO) {
        wr_length = read(f_i->fd, buff, 1);
    } else if (wr_type == WR_FILE_IO) {
        wr_length = fread(buff, 1, block_size, f_i->c_fd);
    } else if (wr_type == WR_MMAP_IO) {
    }
    return wr_length;
}

static int wr_write(char *buff, int block_size, file_info_t *f_i)
{
    int wr_length = 0;
    if (wr_type == WR_COMMON_IO) {
        wr_length = write(f_i->fd, buff, 1);
    } else if (wr_type == WR_FILE_IO) {
        wr_length = fwrite(buff, block_size, 1, f_i->c_fd);
    } else if (wr_type == WR_MMAP_IO) {
    }
    return wr_length;
}

static int get_options(int argc, const char *argv[])
{
    uint8_t *p;
    int i;
    char *tmp_arg;

    for (i = 1; i < argc; i++) {
        p = (uint8_t *) argv[i];

        if (*p++ != '-') {
            sprintf(0, "invalid option: \"%s\"", argv[i]);
            return 1;
        }
        if (*p == '-') {
            p++;
            if (strncmp("help", (char *)p, 4) == 0) {
                wr_show_version = 1;
                wr_show_help = 1;
                return 0;
            }
        }

        while (*p) {
            tmp_arg = NULL;
            switch (*p++) {
            case '?':
            case 'h':
                wr_show_version = 1;
                wr_show_help = 1;
                break;

            case 'v':
                wr_show_version = 1;
                break;

            case 'V':
                wr_show_version = 1;
                break;

            case 'b':
                if (*p) {
                    tmp_arg = (char *)p;
                } else if (argv[++i]) {
                    tmp_arg = (char *) argv[i];
                }
                if (tmp_arg == NULL) {
                    printf("option \"-b\" requires .\n");
                    return WR_ERROR;
                }
                block_size = atoi(tmp_arg);
                goto next;
            case 'i':
                if (*p) {
                    tmp_arg = (char *)p;
                } else if (argv[++i]) {
                    tmp_arg = (char *) argv[i];
                }
                if (tmp_arg == NULL) {
                    printf("option \"-t\" requires .\n");
                    return WR_ERROR;
                }
                input_file = tmp_arg;
                goto next;
            case 'o':
                if (*p) {
                    tmp_arg = (char *)p;
                } else if (argv[++i]) {
                    tmp_arg = (char *) argv[i];
                }
                if (tmp_arg == NULL) {
                    printf("option \"-t\" requires .\n");
                    return WR_ERROR;
                }
                output_file = tmp_arg;
                goto next;
            case 't':
                if (*p) {
                    tmp_arg = (char *)p;
                } else if (argv[++i]) {
                    tmp_arg = (char *) argv[i];
                }
                if (tmp_arg == NULL) {
                    printf("option \"-t\" requires .\n");
                    return WR_ERROR;
                }
                wr_type = atoi(tmp_arg);
                goto next;
              default:
                printf("invalid option: \"%c\"\n", *(p - 1));
                return WR_ERROR;
            }
        }
      next:
        continue;
    }

    return WR_OK;
}

#define WR_VERSION_STRING "0.9.5"
void show_version()
{
    printf("WR version: %s\n", WR_VERSION_STRING);
}

void show_help()
{
    printf("Usage: sendpacket [-?hvV] [-i input_file] [-o output_file]\n"
           "                  [-t type] [-b block_size]\n"
           "       --help print this message and exit\n");
}


int main(int argc, const char *argv[])
{
    file_info_t fp_i, fp_o;
    char *buf = NULL;
    int j;
    int wr_length = 0;
    time_t t_b;
    time_t t_e;
    struct stat64 file_info;

    get_options(argc, argv);
    if (wr_show_help) {
        show_version();
        show_help();
        return 0;
    }
    if (wr_show_version) {
        show_version();
        return 0;
    }
    if (input_file == NULL) {
        show_help();
        return 1;
    } else {
        strcpy(fp_i.str, input_file);
        wr_open(&fp_i, WR_READ);
        stat64(input_file, &file_info);
        file_size = file_info.st_size;
        printf("input file size: %lu Bytes, %lu MB.\n", file_size, file_size >> 20);
        fseek(fp_i.c_fd, 0L, SEEK_SET);
    }
    if (output_file == NULL) {
        show_help();
        return 1;
    } else {
        strcpy(fp_o.str, output_file);
        wr_open(&fp_o, WR_WRITE);
    }

    buf = malloc(block_size);
    t_b = time(NULL);
    for (j = 0; ; j++) {
        wr_length = wr_read(buf, block_size, &fp_i);
        if (wr_length <= 0) {
            printf("wr_length is :%d.\n", wr_length);
            break;
        } else if (wr_length < block_size) {
            printf("last wr_length is :%d.\n", wr_length);
            wr_write(buf, wr_length, &fp_o);
            break;
        }
        wr_write(buf, block_size, &fp_o);
    }
    t_e = time(NULL);
    printf("tb: %lu, te: %lu.\n", t_b, t_e);
    if (t_e > t_b) {
        printf("ok! seconds: %lu, rate: %luMbps.\n", t_e - t_b, (file_size/ (t_e - t_b)) >> 17);
    }
    free(buf);
    wr_close(&fp_i);
    wr_close(&fp_o);
    return 0;
}
