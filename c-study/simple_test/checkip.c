#include <stdio.h>

int checkIpv4(char *ip)
{
    printf("检验对象是：%s\n", ip);
    char *p = ip;               //遍历指针
    char *q = ip;               //字串指针
    int i = 0, s, count = 0;    //i是每个字串的长度，s是字串转化为的整型，count是字串的个数

    if (*p == '.')              //处理特殊情况
        return 0;

    while (*p != '\0') {        //遍历每个字符
        if (*p == '.' || *(p + 1) == '\0') {    //根据'.'将字符串切割出来，最后一个字串根据'\0'识别
            count++;            //计算切割的字串个数

            if (*(p + 1) == '\0') { //处理最后一个字串，‘\0’识别的时候
                i++;
                q = p;
            } else
                q = p - 1;      //‘.切割’

            s = 0;
            int x = -1;
            for (int j = 1; j <= i; j++) {  //将切割出来的字串变成整型
                x = *q - '0';   //字符减'0'变为整型
                if (x < 0 || x > 9) //判断x是否在0～9之间
                    return 0;

                for (int k = 1; k < j; k++) //第几位乘以几-1个10.
                    x *= 10;
                s += x;
                q--;
            }

            if (x == -1)
                return 0;       //防止类型“1...3”出现连续点的情况

            printf("%d\n", s);
            if (s < 0 || s > 255)   //判断切割出来的字串是否在ipv4范围内
                return 0;
            i = 0;
        } else {
            i++;
        }
        p++;
    }
    if (count == 4)             //检查是否是四个字串
        return 1;
    else
        return 0;
}

int main(void)
{
    char ip[] = "1..2.3";
    if (checkIpv4(ip))
        printf("该地址是IPv4地址\n");
    else
        printf("该地址不是IPv4地址\n");
    return 0;
}
