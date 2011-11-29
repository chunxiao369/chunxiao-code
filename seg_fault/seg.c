/**
 * @file seg.c
 * @Synopsis  userful when found where is segment fault take place
 * @author xu.chunxiao@embedway.com
 * @version 1.0.0
 * @date 2011-11-25
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#define __USE_GNU
#include <ucontext.h>
#include <execinfo.h>
char main_status = 0;

/***********************
 * 此函数产生一个段错误
 * *********************/
void dummy_func(void)
{
    printf("hello world\n");
    char *p = 0x1234;             //0地址
    free(p);
    //*p = 0x1a;                  //对0地址写入数据，出现段错误
    return;
}

/**
 * @Synopsis  OnSIGSEGV 
 *
 * @Param signum
 * @Param info
 * @Param ptr
 */
//#define PPC
void OnSIGSEGV(int signum, siginfo_t * info, void *ptr)
{
    void *array[25];
    int i;
    int nSize = backtrace(array, sizeof(array) / sizeof(array[0]));
    for (i = nSize - 3; i > 2; i--) {
        /* 头尾几个地址不必输出 */
        /* 对array修正一下，使地址指向正在执行的代码 */
        printf("before signal[%d] catched when running code at %p\n", signum, (char *)array[i] - 1);
    }

    if (NULL != ptr) {
        ucontext_t *ptrUC = (ucontext_t *) ptr;
//#ifdef PPC
        struct pt_regs *pgregs = ptrUC->uc_mcontext.regs;
        int nip = pgregs->nip;
        int dar = pgregs->dar;
        printf("ccc signal[%d] catched when running code at %x\n", signum, nip);    /* 出错地址 */
        //printf("ccc signal[%d] catched when dar is %x\n", signum, dar);    /* 出错地址 */
        //printf(" Failed at address = %p \n", (unsigned int *)(info->si_addr));
        //printf(" Failed Op-code    = 0x%x at address = %p \n", *(unsigned int *)(info->si_addr), (unsigned int *)(info->si_addr));
//#endif
#if I386-ppppp
        int *pgregs = (int*)(&(ptrUC->uc_mcontext.gregs));
        int eip = pgregs[REG_EIP];
        if (eip != (int)array[i]) { /* 有些处理器会将出错时的 EIP 也入栈 */
            printf("bbb signal[%d] catched when running code at %p\n", signum, (char *)array[i] - 1);
        }
        printf("ccc signal[%d] catched when running code at %x\n", signum, eip);    /* 出错地址 */
#endif
    } else {
        printf("ddd signal[%d] catched when running code at unknown address\n", signum);
    }
    abort();
}

void deal(void)
{
    int a;
    char buffer[255];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "cd /mnt/D-disk/chunxiao-code/seg_fault");
    a = system(buffer);

    /*
     *此延时很重要，如果不加延时，ctrl+c的信号无法及时处理（即ctrl+c失效），程序将循环加载
     * 如果不加延时，程序又重新执行后面./test1语句将重新加载应用程序了
     */
    //sleep(1);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "./xuexe");
    a = system(buffer);
    printf("xxxxxxxxxxxxxxx\n");
    {
        int i;
        void *array[25];        /* 25 层，太够了 : )，你也可以自己设定个其他值 */
        int nSize = backtrace(array, sizeof(array) / sizeof(array[0]));
        for (i = nSize - 3; i >= 2; i--) {
            /* 头尾几个地址不必输出，看官要是好奇，输出来看看就知道了 */
            /* 修正array使其指向正在执行的代码 */
            printf("SIGSEGV catched when running code at %p\n", (char *)array[i] - 1);
        }
        abort();
    }

    if (1 == main_status)
        exit(0);
}

/**************************
 * 捕捉到ctrl+c信号的处理函数
 **************************/
void ctrl_c_func(int signo)
{
    printf("stop the demo\n");
    main_status = 1;            //置标志位
    exit(0);
}

int ctrl_c_func_init(void)
{
    int ret = 0;
    struct sigaction act;

    act.sa_handler = ctrl_c_func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    ret = sigaction(SIGINT, &act, NULL);
    return ret;
}

/****************************
 * main主程序
 ****************************/
int main(int argc, char **argv)
{
    //signal(SIGSEGV, &deal);   //捕捉SIGSEGV信号
//    ctrl_c_func_init();  //如果不用上面捕捉，调用这个函数也行
    struct sigaction act;

    int sig = SIGSEGV;

    sigemptyset(&act.sa_mask);
    act.sa_sigaction = OnSIGSEGV;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(sig, &act, NULL) < 0) {
        perror("sigaction:");
    }

    signal(SIGINT, &ctrl_c_func);   //捕捉SIGINT信号

    while (1) {
        if (1 == main_status) {
            exit(0);
        }
        dummy_func();           //调用产生segment fault函数
    }
    return 0;
}

