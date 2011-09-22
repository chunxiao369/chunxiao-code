#include <limits.h>
#include <pthread.h>
#include <error.h>
#include <stdio.h>
#include <string.h>

void *thread_routine()
{
    //栈大小为16M,如果直接分配16M的栈空间，会出现段错误 ，因为栈中还有其它的
    //变量要放署
    char p[1024*1024];
    int i = 1024*1024;

    printf ("The thread is here\n");
    //确定内存分配的确成功了
    while(i--)
    {
        p[i] = 3;      
    }

    printf( "Get xxM Memory!!!\n");
    /*    
    //分配更多的内存(如果分配1024*1020*512的话就会出现段错误）
    char p2[ 1024 * 1020 + 256 ];
    memset( p2, 0, sizeof( char ) * ( 1024 * 1020 + 256 ) );
    printf( "Get More Memory!!!\n" );
    */
    return NULL;

}

int main (int argc, char *argv[])
{
    pthread_t thread_id;
    pthread_attr_t thread_attr;
    //size_t stack_size;
    int status;
    int i = 5;

    status = pthread_attr_init (&thread_attr);
    if (status != 0)
        printf("Create attr");

    status = pthread_attr_setdetachstate (
        &thread_attr, PTHREAD_CREATE_DETACHED);

    if (status != 0)
        printf("Set detach");
    //通常出现的问题之一，下面的宏没有定义
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    printf("xu chunxiao add!\n");
    //得到当前的线程栈大小
    status = pthread_attr_getstacksize (&thread_attr, &stack_size);
    if (status != 0)
        printf("error\n"); 
    printf ("Default stack size is %u; minimum is %u\n",
        stack_size, PTHREAD_STACK_MIN);

    //设置当前的线程的大小
    status = pthread_attr_setstacksize (
        &thread_attr, PTHREAD_STACK_MIN*1024);
        
    if (status != 0)
        printf("error\n"); 
    //得到当前的线程栈的大小
    status = pthread_attr_getstacksize (&thread_attr, &stack_size);
    if (status != 0)
        printf("Get stack size");
    printf ("Default stack size is %u; minimum is %u\n",
        stack_size, PTHREAD_STACK_MIN);
#endif
    printf("xu chunxiao add last!\n");
    //分配5个具有上面的属性的线程体
    while(i--)
    {
        status = pthread_create (
            &thread_id, &thread_attr, thread_routine, NULL);
        if (status != 0)
            printf("error\n"); 
    }

    getchar();
    printf ("Main exiting\n");
    pthread_exit (NULL);
    return 0;
}

