#include	<sys/types.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<dirent.h>
#include	<sys/wait.h>
#include	<signal.h>
#define  MAXLINE 1024
#define  err_sys(fmt, args...) printf(fmt, ##args)
int main(void)
{
	char	buf[MAXLINE];
	pid_t	pid;
	int		status;
    /*
	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal error");
    */
    memset(buf, 0, MAXLINE);
	printf("cli>");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

		if ( (pid = fork()) < 0)
			err_sys("fork error");

		else if (pid == 0) {		/* child */
			execlp(buf, buf, (char *) 0);
			err_sys("couldn't execute: %s", buf);
			exit(127);
		}

		/* parent */
		if ( (pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");
		printf("cli>");
	}
	exit(0);
}
/*
void sig_int(int signo)
{
	printf("interrupt\n%% ");
}
*/
