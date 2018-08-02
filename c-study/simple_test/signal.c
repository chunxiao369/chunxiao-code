#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h> 
#include <signal.h> 

static void on_signal ( int signo )
{
    static int count = 0;
    FILE *fp;

    /*
     * ........
     */
    printf("cxxu add count = %u\n", count++ );
    fp = fopen( "/tmp/xxxx", "rw" );
    if ( fp == NULL )
    {
        printf( "[ FAILED ] open config file error\n" );
        return;
    }
    else 
    {
        printf("[ OK ] open config file right!\n");
    }

    printf("cxxu add count = %u\n", count++ );
    return;
}

static void on_signal2 ( int signo )
{
    static int count = 0;

    /*
     * ........
     */
    printf("aaaaaaaaaaaaaaaaa%u\n", count++ );
    sleep(1);
    printf("bbbbbbbbbbbbbbbbb%u\n", count++ );
    alarm(2);
    return;
}

int main()
{
    int aaa = 16;
    int a = 0x80808080;
    printf("%d\n", !!aaa);
    a = a >> 1;
    printf("%x\n", a);
    return 0;
    /* 
    signal( SIGKILL, on_signal );
    signal( SIGSTOP, on_signal );
    signal( SIGTERM, on_signal );
    */
    signal( SIGHUP, on_signal );
    signal( SIGINT, on_signal );
    signal( SIGALRM, on_signal2 );
    alarm(1);
    while (1) {
        aaa++;
        if (aaa % 8 == 0) {
            printf("line: %d.\n", __LINE__);
            signal( SIGALRM, on_signal2 );
            alarm(1);
            continue;
        }
        if (aaa % 4 == 0) {
            printf("line: %d.\n", __LINE__);
            alarm(0);
        }
        sleep(1);
    }
    return 0;
}

