#include <stdio.h>
int main()
{
    unsigned long long a = 0x123456789abcdef0;
    unsigned char *buf;
    unsigned char temp[8];
    int i;

    //buf = &a;
    //buf = &a;
    
    //printf("a: %s\n",buf);
    //sprintf(temp,"%llu\n",a);
    //printf("temp: %s\n",temp);

    buf = temp;

    *(unsigned long long *)buf = a;

    for (i=0; i<8; i++){
        printf("%02x", temp[i]);
    }    
    printf("\n");
    return 0;
}

