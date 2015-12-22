#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    int rule = 80001;
    int i;
    long abc;
    char buf[256] = {0};
    char buf2[256] = {0};
    for (rule = 80001; rule <= 82000; rule++) {
        abc = random();
        memset(buf, 0, 256);
        sprintf(buf, "%lx", abc);
        for (i = 0; i < strlen(buf); i++) {
            //printf("%02x", (unsigned char)buf[i]);
            sprintf(buf2 + i * 2, "%02x", (unsigned char)buf[i]);
        }
        //printf("%s\n", buf2);
        printf("add rule %d floatud \"6166696374726C%s\"", rule, buf2);
        printf("\n");
    }
}
