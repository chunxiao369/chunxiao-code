#include <stdio.h>
#include <time.h>

#define DATA_SIZE  (1024ull*1024ull*1024ull)
#define DATA_LEN   (1024 * 1024)
#define DATA_TIMES (DATA_SIZE / DATA_LEN)

int main()
{
    FILE *fp1, *fp2;
    char buf[DATA_LEN] = {0};
    int i, j;
    int length;
    time_t t_b;
    time_t t_e;

    fp1 = fopen("data.in", "rb");
    fp2 = fopen("data.out", "wb");
    t_b = time(NULL);
    for (j = 0; j < DATA_TIMES; j++) {
        length = fread(buf, DATA_LEN, 1, fp1);
        if (length < 1) {
            printf("length is :%d.\n", length);
            break;
        }
#if 0
        for (i = 0; i < DATA_LEN; i++)
            buf[i]++;
#endif
        fwrite(buf, DATA_LEN, 1, fp2);
    }
    t_e = time(NULL);
    printf("ok! seconds: %lu, rate: %lluMbps.\n", t_e - t_b, (DATA_SIZE / (t_e - t_b)) >> 17);
    fclose(fp1);
    fclose(fp2);
    return 0;
}
