#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* only malloc not free 
 * os is running very well
 */
#define MAX_SIZE (2 << 20)
int main()
{
    char *p[1024] = {0};
    int i;
    printf("%d.\n",inet_addr("ip=202.255.255.255"));
    for (i = 0; i < 1024; i++) {
        usleep(10000);
        p[i] = malloc(MAX_SIZE);
        if (p[i] == NULL) {
            printf("null, return, %d\n", i);
            return 0;
        }
    }

    for (i = 0; i < 1024; i++) {
        memset(p[i], 0, MAX_SIZE);
        memcpy(p[i], &i, 100);
        usleep(10000);
        if (p[i] == NULL) {
            printf("null\n");
        } else {
            //free(p[i]);
        }
    }
    return 0;
}

