#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>

#define _(x)         x
#define Conn(x,y)    x##y
//#define ToChar(x)    #@x
#define ToString(x)  #x

char * func()
{
    return "only works for simple strings";
}

void fun(int a, ...) 
{ 
    int *temp = &a;
    int i;
    temp++; 
    for (i = 0; i < a; ++i) 
    { 
        printf("%d ", *temp);
        temp++; 
    }
    printf("\n");
}

typedef struct _xu
{
    int a;
    int b;
    int c;
}xudef;

int main()
{
    char temp[128] = {0};
    char *string = "9 18 2 5 PKTNUM 0";
    printf("%s.\n", strstr(string, "PKTNUM"));
    char *p = strstr(string, "PKTNUM");
    printf("%p, %p.\n", string, p);

    strncpy(temp, string, p - string);
    printf("%s\n", temp);

    p = strtok(temp, " ");
    while (p) {
        printf("p: %s.\n", p);
        p = strtok(NULL, " ");
    }

    return 0;
}

