/*
 * 生成一个文件
 * 每一行是一个数字
 * 从 1 排到 8192
 * 为smartTAP生成8192个vlan写的
 */

#include <stdio.h>
int main()
{
    FILE *fp;
    int a = 1;
    char str[12];

    if((fp=fopen("a.txt", "w"))==NULL)
    {
        printf("not open\n");
        return 1;
    }
    while (a < 8193)
    {
        sprintf(str, "%06d", a);        
        fputs(str, fp);
        fputs("\n", fp);
        a++;
    }
    
    fclose(fp);
    
    return 0;
}

