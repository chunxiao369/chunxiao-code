#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define MAXDATASIZE  128
#define TEMP_FILE    "temp_conf"
#define HARD_ADDR    "    hardware ethernet"
#define FIXED_ADDR   "    fixed-address"
#define OPTION       "    option octeon-boot-command \"bootoct 0 coremask=0xff ip=$(ipaddr)\";"
#define FILE_NAME    "    filename \"pa_main.strip\";"

void write_config_info(char *readoption, char *mac_string, char *ip_string, FILE *fp)
{
    char buf[MAXDATASIZE] = {0};

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "%s %s {\n", "host", readoption);
    fprintf(fp, "%s", buf);

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "%s %s;\n", HARD_ADDR, mac_string);
    fprintf(fp, "%s", buf);

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "%s %s;\n", FIXED_ADDR, ip_string);
    fprintf(fp, "%s", buf);

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "%s\n", OPTION);
    fprintf(fp, "%s", buf);

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "%s\n", FILE_NAME);
    fprintf(fp, "%s", buf);

    fprintf(fp, "%s", "}\n");
}

int update_dhcpd_conf_file(uint32_t ipaddr, uint8_t *mac,\
                           int slot_id, int module_id, char* filename)
{
    struct in_addr inp;
    FILE *fp;
    FILE *new_fp;
    char readoption[64] = {0};
    char buf[MAXDATASIZE] = {0};
    char mac_string[64] = {0};
    int found = 0;
    int get_v;

    inp.s_addr = ntohl(ipaddr);
    printf("ipaddr: %s.\n", inet_ntoa(inp));
    sprintf(mac_string, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf("   mac: %s.\n", mac_string);

    sprintf(readoption, "slot%d_module%d", slot_id, module_id);

    if (access(filename, F_OK)) {
        printf("file %s not exit.\n", filename);
        return 1;
    }
    if ((fp = fopen(filename, "r")) == NULL)
        return 2;

    if (access(TEMP_FILE, F_OK) == 0) {
        unlink(TEMP_FILE);
    }
    if ((new_fp = fopen(TEMP_FILE, "w")) == NULL)
        return 2;
    /* every slot module option */
    while (fgets(buf, MAXDATASIZE, fp)) {
        if (buf[0] == '\n') {
            continue;
        }
        if (buf[0] == '#') {
            fprintf(new_fp, "%s", buf);
            continue;
        }
        if (strstr(buf,"host slot") == NULL) {
            fprintf(new_fp, "%s", buf);
            continue;
        }
        if (strstr(buf, readoption) == NULL) {
            fprintf(new_fp, "%s", buf);
            continue;
        }
        found = 1;

        write_config_info(readoption, mac_string, inet_ntoa(inp), new_fp);
        while((get_v = fgetc(fp)) != EOF) {
            if (get_v == (int)'}')
                break;
        };

        memset(buf, 0, MAXDATASIZE);
    }
    if (found == 0) {
        write_config_info(readoption, mac_string, inet_ntoa(inp), new_fp);
    }
    fclose(fp);
    fclose(new_fp);
    unlink(filename);
    rename(TEMP_FILE, filename);
    return 0;
}

typedef struct test_s_{
    int a;
    int b[0];
}test_t;

int main()
{
    /*
    uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    update_dhcpd_conf_file(32, mac, 1, 1, "dhcpd.conf");
    */
    /*
    char *p = NULL;
    p = malloc(1024 << 21);
    free(p);
    while(1);
    return 0;
    */
    test_t test;
    printf("size if %d.\n", sizeof(test_t));
    printf("s is %p.\n", &test);
    printf("b is %p.\n", test.b);
    return 0;
}


