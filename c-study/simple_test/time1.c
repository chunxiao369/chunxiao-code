#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <arpa/inet.h>

typedef struct _sccp {
    int a;
    int b;
    int c;
} sccp_t;

sccp_t temp;
sccp_t * function()
{
    temp.a = 6;
    temp.b = 7;
    temp.c = 8;

    return &temp;
}

int function_2()
{
    int a;
    int b;
    int c;
    
    a = 9;
    b = 10;
    c = 11;

    c = b + c;
    return a;
}
/*
    test for predefined macro
*/
int main()
{
    time_t biggest;
    time_t middle;
    time_t smallest;
    sccp_t *temp;

    char buf[64] = {0};
    time_t random_time = 0;
    uint8_t xx[4] = {0x80, 0xbd, 0xa5, 0x06};
    struct tm tm_s;
    struct tm tm_d;

    random_time = ntohl(*(uint32_t*)xx);
    localtime_r(&random_time, &tm_s);
    printf("ud: %4d-%02d-%02d\n", tm_s.tm_year + 1900, tm_s.tm_mon + 1, tm_s.tm_mday);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &tm_s);
    printf("strftime buf: %s\n", buf);

    memset(&tm_d, 0, sizeof(struct tm));
    strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_d);
    strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &tm_d);
    printf("strptime-strftime buf: %s\n", buf);

    asctime_r(&tm_s, buf);
    printf("buf: %s\n", buf);
    printf("buf: %s\n", ctime((const time_t *)&xx));
    printf("buf: %s\n", asctime((const struct tm *)&tm_s));
    return 0;

    printf("##############date: %s, time: %s\n", __DATE__, __TIME__);
    printf("##############date: %s, time: %d\n", "aa", __LINE__);
    printf("##############file: %s, func: %s\n", __FILE__, __FUNCTION__);
    //printf("sizeof time_t: %d\n", sizeof(time_t));
    biggest  = 0x7fffffff;
    middle   = 0xffffffff;
    smallest = 0x80000000;
    
    printf("biggest = %s", ctime(&biggest));
    printf("biggest = %s", asctime(gmtime(&biggest)));
    
    printf("middle = %s", ctime(&middle));
    middle = 0;
    printf("middle = %s", ctime(&middle));

    printf("smallest = %s", ctime(&smallest));
    
    temp = function();
    function_2();
    printf("a: %d, b: %d, c: %d.\n",temp->a, temp->b, temp->c);
    return 0;
}

