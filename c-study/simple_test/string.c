
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
    struct utsname name;
    char *s;
    int a = 1; 
    int b = 2; 
    int c = 3; 
    int d = 4; 
    xudef *chunxiao;

    char *errmsg = _("Device not found");

    int aa = Conn(1,2);
    char *msg1 = "abcdef";//Conn(abc,def);
    //char msg2 = ToChar(a);
    char *msg3 = ToString(abcdef);

    fun(4, a, b, c, d); 
    if (uname(&name) == -1) {
       return 1;
    }
    s = name.release;
    printf("size of is %d, %d.\n", sizeof(*chunxiao), sizeof(chunxiao));
    printf("just for test: %s.\n", func());
    printf("release: %s, errmsg: %s.\n", s, errmsg);
    printf("aa, %d, string1: %s.\n", aa, msg1);
    //printf("string2: %c.\n", msg2);
    printf("string3: %s.\n", msg3);
    s = strdup("aaaaaaaaaa");
    free(s);
    return 0;
}

