
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() 
{          
    char buf[100]; 
    int aaa;
    char abort_frm[20],abort_pps[20]; 
    char test[20];
    /*
    printf("please input aaa.\n");
    scanf("%x", &aaa);
    printf("input aaa is %X.\n", aaa);
    */
    strcpy(buf, "   Pox Rx Abort Frm     1000 pkts   2345 pps");
    sscanf(buf, "%*[^0-9]%[0-9]%*[^0-9]%[0-9]%[^0-9]", abort_frm, abort_pps, test);
    printf("%s   %s   %s\n", abort_frm, abort_pps, test);
    
    return 0;      
}    




