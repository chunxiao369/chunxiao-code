#include <stdio.h>
#define size 1024*1024*1024
#define LEN 1024
int main()
{
    FILE *fp1, *fp2;
    char buf[LEN] = {0};
    int i, j;
    int length;
    fp1 = fopen("data.in", "rb");
    fp2 = fopen("data.out", "wb");
    for (j = 0; j < 1024 * 1024; j++) {
        length = fread(buf, LEN, 1, fp1);
        if (length < 1) {
            printf("length is :%d.\n", length);
            break;
        }
        for (i = 0; i < LEN; i++)
            buf[i]++;
        fwrite(buf, LEN, 1, fp2);
    }
    printf("ok!\n");
    fclose(fp1);
    fclose(fp2);
    return 0;
}
