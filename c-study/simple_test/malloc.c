#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* only malloc not free 
 * os is running very well
 */
int main()
{
    char *p[1024];
    int i;
    printf("%d.\n",inet_addr("ip=202.255.255.255"));
    for (i = 0; i < 1024; i++)
        p[i] = malloc(1024 * 1024);
    
    return 0;
}

