/**
 * @file unicode_dis.c
 * @Synopsis  unicode display sample
 * @author xu.chunxiao@embedway.com
 * no0.9.2
 * @date 2014-11-29
 */
#include <stdio.h>
#include <stdint.h>
#include <locale.h>
#include <wchar.h>
int main()
{
    const wchar_t *abc = L"我而做地铁去紧路上舀蜀";
    char buff2[256] = {0};
    unsigned long length = wcslen(abc);
    unsigned long i;
    uint32_t *buff = NULL;
    if (!setlocale(LC_CTYPE, "")) {
        printf("errrrrrrr\n");
        return 0;
    }

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
