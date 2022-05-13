/**
 * @file unicode_dis.c
 * @Synopsis  unicode display sample
 * @author xu.chunxiao@embedway.com
 * no0.9.2
 * @date 2014-11-29
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <iconv.h>
#include <errno.h>

int main()
{
    char a[256] = {0};
    strcpy(a, "333中国银行222");
    //strcpy(a, "中国银行");
    char *a_ptr = a;
    //const wchar_t *abc = L"我而做地铁去紧路上舀蜀";
    const wchar_t *abc = L"中国银行";
    char buff2[256] = {0};
    char *xx = buff2;
    unsigned long length = wcslen(abc);
    unsigned long i;
    uint32_t *buff = NULL;
    size_t len = 0;
    size_t len_left = 256;
    size_t len_out = 0;
    iconv_t cd;
#if 0
    if (!setlocale(LC_CTYPE, "")) {
        printf("errrrrrrr\n");
        return 0;
    }
#endif
    len = strlen(a);
    printf("inpurt len: %ld.\n", len);
    for (i = 0; i < len; i++) {
        printf("%02x ", (uint8_t)a[i]);
    }
    printf("\n");
    cd = iconv_open("UTF-16BE","UTF8");
    len_out = iconv(cd, &a_ptr, &len, &xx, &len_left);
    if (-1 == len_out) {
        printf("error, len: %ld, %s\n", strlen(buff2), strerror(errno));
        return 0;
    }
    printf("xxxxxxxxxxxxx, len: %ld, %ld\n", strlen(buff2), len_left);
    iconv_close(cd);
    //for (i = 0; i < strlen(buff2); i++) {
    for (i = 0; i < (int)(xx - buff2); i++) {
        printf("%02x ", (uint8_t)buff2[i]);
    }
    printf("\n");
    //return 0;
#if 0
    size_t iconv(iconv_t cd,
                    char **inbuf, size_t *inbytesleft,
                    char **outbuf, size_t *outbytesleft);
#endif
    printf("%ls\n", abc);
    printf("%lu\n", length);
    buff = (uint32_t *)abc;
    for (i = 0; i < length; i++) {
        printf("%08x ", buff[i]);
    }

    printf("\n");

    buff2[0] = 0x11;
    buff2[1] = 0x62;
    buff2[2] = 0x00;
    buff2[3] = 0x00;

    buff2[4] = 0x0c;
    buff2[5] = 0x80;
    buff2[6] = 0x00;
    buff2[7] = 0x00;

    buff2[8] = 0x5a;
    buff2[9] = 0x50;
    buff2[10] = 0x00;
    buff2[11] = 0x00;

    buff2[12] = 0x30;
    buff2[13] = 0x57;
    buff2[14] = 0x00;
    buff2[15] = 0x00;

    buff2[16] = 0xc1;
    buff2[17] = 0x94;
    buff2[18] = 0x00;
    buff2[19] = 0x00;

    buff2[20] = 0xbb;
    buff2[21] = 0x53;
    buff2[22] = 0x00;
    buff2[23] = 0x00;

    buff2[24] = 0x27;
    buff2[25] = 0x7d;
    buff2[26] = 0x00;
    buff2[27] = 0x00;

    buff2[28] = 0xef;
    buff2[29] = 0x8d;
    buff2[30] = 0x00;
    buff2[31] = 0x00;

    buff2[32] = 0x0a;
    buff2[33] = 0x4e;
    buff2[34] = 0x00;
    buff2[35] = 0x00;

    printf("%ls\n", (wchar_t *)buff2);

    return 0;
}
