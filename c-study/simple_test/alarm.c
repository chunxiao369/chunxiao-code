#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
void sigalrm_fn(int sig)
{
    printf("alarm!\n");
    alarm(2);
    return;
}

int main(void)
{
    //signal(SIGALRM, sigalrm_fn);
    struct sigaction alr;

    memset(&alr,0,sizeof(struct sigaction));

    alr.sa_handler = sigalrm_fn;
    alr.sa_flags = SA_NOMASK;
    alr.sa_restorer = NULL;
    
    sigaction(SIGALRM, &alr, NULL);
    alarm(1);
    while(1) 
        pause();
}

