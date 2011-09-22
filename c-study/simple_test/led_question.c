
/*
question

有100盏灯，从1~100编上号，开始时所有的灯都是关着的，
第一次，把所有编号是1的倍数的灯的开关状态改变一次；
第二次，把所有编号是2的倍数的灯的开关状态改变一次；
第三次，把所有编号是3的倍数的灯的开关状态改变一次；
以此类推，直到把所有编号是100得倍数的灯的开关状态改变一次。
问，此时所有开着的灯的编号。 
*/

#include <stdio.h>
#include <string.h>

int led[100];

void show_led()
{    
    int i;
    for (i = 0; i < 100; i++)
    {
        if (i % 10 == 0)
            printf("\n");
        printf("%x ", led[i] & 0x1);
    }
    printf("\n\n");
}
int main()
{
    int i;
    int j;

    memset(led, 0, sizeof(led));
    show_led();

    for (i = 1; i <= 100; i++)
    {
        for (j = 0; j < 100; j++)
        {
            if ((j + 1) % i == 0)
                led[j] = ~led[j];
        }
    }

    show_led();
    return 0;
    
}
