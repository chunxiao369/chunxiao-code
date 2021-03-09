/*
 * 生成一个文件
 * 每一行是一个数字
 * 从 1 排到 8192
 * 为smartTAP生成8192个vlan写的
 */

#include <stdio.h>
#include <glib.h>
int main()
{
    FILE *fp;
    int a = 1;
    int c = 1;
    int b;
    char str[12];
    GHashTable *g_table = NULL;

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
        if ( a == 1024) {
            b = c;
        }
        a++;
        c++;
    }
    c = 0;
    
    g_table = g_hash_table_new(NULL, NULL);
    fclose(fp);
    
    return 0;
}

