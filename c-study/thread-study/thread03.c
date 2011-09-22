/*thread_example.c :  c multiple thread programming in linux 
  *author : falcon 
  *E-mail : tunzhj03@st.lzu.edu.cn 
  
This is main here,I am trying my best to creat the process....
thread_1 created success.:)
thread_2 created success.:)
This is main here,I am waiting for the child process to stopback...
thread1 : I'm thread 1
thread1 : number = 0
Thread2: I'm thread 2
thread2 : number = 1
thread1 : number = 2
thread2 : number = 3
thread1 : number = 4
thread1 : number = 5
thread2 : number = 6
thread1 : number = 7
thread1 : number = 8
thread2 : number = 9
thread1 : number = 10
thread1: Is father(main) waiting for me?
thread_1 end.
thread2: Is father(main) waiting for me?
thread_2 end.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <sys/time.h> 
#include <string.h> 

#define MAX 10

pthread_t thread[2]; 
pthread_mutex_t mut; 
int number=0, i; 

void *ptr;
void *thread1() 
{ 
        printf ("thread1 : I'm thread 1\n"); 
        void *aa = malloc(128);
        free(ptr);
        for (i = 0; i < MAX; i++) 
        { 
                printf("thread1 : number = %d\n",number); 
                printf("aa ptr %p.\n", aa);
                void *bb = malloc(128);
                printf("bb ptr %p.\n", bb);
                pthread_mutex_lock(&mut); 
                        number++; 
                pthread_mutex_unlock(&mut); 
                free(aa);
                sleep(1); 
        } 

        printf("thread1 :主函数在等我完成任务吗？\n"); 
        pthread_exit(NULL); 
} 

void *thread2() 
{ 
        printf("thread2 : I'm thread 2\n"); 

        for (i = 0; i < MAX; i++) 
        { 
                printf("thread2 : number = %d\n",number); 
                pthread_mutex_lock(&mut); 
                        number++; 
                pthread_mutex_unlock(&mut); 
                sleep(1); 
        } 


        printf("thread2 :主函数在等我完成任务吗？\n"); 
        pthread_exit(NULL); 
} 

void thread_create(void) 
{ 
        int temp; 
        ptr = malloc(128);
        printf("ptr %p.\n", ptr);
        memset(&thread, 0, sizeof(thread));          //comment1 
        /*创建线程*/ 
        if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)       //comment2 
                printf("线程1创建失败!\n"); 
        else 
                printf("线程1被创建\n"); 
        if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)  //comment3 
                printf("线程2创建失败"); 
        else 
                printf("线程2被创建\n"); 
} 

void thread_wait(void) 
{ 
	    printf("==================thread_wait start=================\n");
        /*等待线程结束*/ 
        if(thread[0] !=0) {                   //comment4 
                pthread_join(thread[0],NULL); 
                printf("线程1已经结束\n"); 
        }
        if(thread[1] !=0) {                //comment5 
                pthread_join(thread[1],NULL); 
                printf("线程2已经结束\n"); 
        }
        printf("=================thread_wait end==================\n");
} 

int main(int argc, char *argv[]) 
{ 
        /*用默认属性初始化互斥锁*/ 
        pthread_mutex_init(&mut,NULL); 

        printf("我是主函数哦，我正在创建线程，呵呵\n"); 
        thread_create(); 
        printf("我是主函数哦，我正在等待线程完成任务阿，呵呵\n"); 
        //while(1);
        thread_wait(); 

        return 0; 
} 

/*
int main(int argc, char *argv[])
{
  
  system("PAUSE");	
  return 0;
}
*/

