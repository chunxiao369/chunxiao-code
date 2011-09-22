#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#define __USE_ISOC99
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define CLI_PROMPT  "CLI> "
#define EOT         4
#define ENQ         5

enum {
	_COMMON = 0,
	_EXIT,
	_EXIT_SHELL,
	_OMADMIN,
	_RESTART
};

FILE *fp;
char buf[1024];
int console = 0;
int interactive=1;
int  c_flag=0;
int rc = 0;
struct termios tp;

void setalarm(n)
{
	if (!console)
		alarm(n);
}

void exec_shell()
{
	if (console) {
		execl("/bin/login", "login", "root", (char *) 0);
	}
}


void hd_alarm()
{
	if(isatty(0)) tcsetattr(0,TCSANOW,&tp);
	fprintf(stderr, "\n");
	fprintf(stderr, "CLI timed out after 30 minutes..\n");
	exit(0);
}

void hd_sigpipe()
{
	if(isatty(0)) tcsetattr(0,TCSANOW,&tp);
	fprintf(stderr, "\n");
	fprintf(stderr, "Server restart,disconnect.\n");
	exit(0);
}
void output(int sd, char *s)
{
	char *p;

	write(sd, s, strlen(s));
	while (fgets(buf, sizeof(buf), fp)) {
		if (*buf==EOT) { 
			rc = atoi(buf + 1);
			break;
		} else if (*buf==ENQ) { 
			if( (p=strrchr(buf,'\n'))!=NULL) *p='\0';
			fputs(buf+1, stdout);
			fgets(buf,sizeof(buf),stdin);
			write(sd, buf, strlen(buf));
		} else {
			fputs(buf, stdout);
		}
	}
	return;
}

int open_socket()
{
	int sockfd;
	const char *skaddr = "/tmp/mysocket";
	struct sockaddr_un saun;

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("create socket");
		return (-1);
	}

	saun.sun_family = AF_UNIX;
	strcpy(saun.sun_path, skaddr);

	if (connect(sockfd, (struct sockaddr *) &saun, sizeof(saun)) < 0) {
		//perror("connect socket");
		return (-1);
	}
	return (sockfd);
}

void print_logo(FILE * sd, int p)
{
	char buf[1024];
	while (fgets(buf, sizeof(buf), sd)) {
		if (*buf == EOT)
			break;
		if (p)
			fputs(buf, stdout);
	}
}


char *getcmdline()
{
	char *s;

	if(!interactive) {
		fprintf(stdout,"result=%d\n",rc);
		fflush(stdout);
		fgets(buf,sizeof(buf),stdin);
		return buf;
	}

	setalarm(30 * 60);
	s = readline(CLI_PROMPT);
	setalarm(0);
	if (s == NULL || *s == '\0')
		return NULL;
	add_history(s);
	strcpy(buf, s);
	free(s);
	strcat(buf, "\n");
	return buf;
	
}

int parse(char *buf)
{
	char *p;

	p = buf;

	while(isblank(*p)) { p++; }

	if(*p=='\n') return _COMMON;
	
	if (strncasecmp(p, "restart",7)==0) {
		p+=7;
		return ( isspace(*p) ?  _RESTART: _COMMON);
	} 
	else if (!strncasecmp(p, "OMADMIN",3)) {
		p+=3;
		return ( isspace(*p) ?  _OMADMIN: _COMMON);
	}

	if (!strncasecmp(p, "exit",4)) {
		p+=4;
		while(isblank(*p))p++;
		if (*p == '\n')
			return _EXIT;
		if (!strncasecmp(p, "shell",5)) {
			return _EXIT_SHELL;
		}
		return _EXIT;
	}

	return _COMMON;

}

int do_command(int sd, char *s)
{
	int r;

	r = parse(s);

	switch (r) {
	case _COMMON:
		output(sd, s);
		break;
	case _RESTART:
		write(sd,s,strlen(s));
		break;
	case _OMADMIN:
		if ( isatty(0) ) system("/usr/local/bin/omadmin");
		r=_COMMON;
		break;
	case _EXIT:
	case _EXIT_SHELL:
		break;
	}
	return r;
}

int do_session(int sd)
{
	int command;
	char *s;
	while (1) {
		s = getcmdline();
		if (s == NULL) 
			continue; 
		command = do_command(sd, s);
		if (command == _EXIT && console)
			continue;
		if (command != _COMMON )
			break;
	}
	return command;
}

int init(int argc,char **argv)
{
	int temp;
	int  i;

	interactive = 1;

	while ((temp = getopt(argc, argv, "cn")) >0 ) {
		switch (temp) {
		case 'c':
			interactive = 0;
			c_flag=1;
			break;
		case 'n':
			interactive = 0;
			break;
		default:
			break;
		}
	}

	if (isatty(0))
	{
		console = ! (strcmp(ttyname(0), "/dev/ttyS0")&&(strcmp(ttyname(0), "/dev/console")));
	}

	if( c_flag) {
		buf[0] = '\0';
		i = optind;
		while (argv[i]) {
			strcat( buf, argv[i++]);
			strcat(buf, " ");
		}
		if(*buf=='\0') strcpy(buf,"help\n"); else strcat(buf, "\n");
	}

	if(isatty(0)) tcgetattr(0,&tp);
	signal(SIGHUP,SIG_DFL);
	signal(SIGALRM, hd_alarm);
	signal(SIGPIPE,  hd_sigpipe);
	if(interactive) { 
		rl_variable_bind("disable-completion", "On");
		using_history();
		stifle_history(20);
	}
	return(0);

}

int main(int argc, char **argv)
{
	int sd;
	int status;

	init(argc,argv);

	do {
		sd = open_socket();
		if ( sd >= 0) break; 
		sleep(1);
	} 
	while (console);


	if ( sd < 0) exit(2);

	if ((fp = fdopen(sd, "rw")) == NULL) {
		perror("fdopen");
		exit(1);
	}

	signal(SIGINT, SIG_IGN);

	print_logo(fp, interactive);

	if ( c_flag ) {
		do_command(sd, buf);
	} else {
		status = do_session(sd);
		rc=0;
	}

	close(sd);

	if ( isatty(0) && status == _EXIT_SHELL)
		exec_shell();

	if( c_flag ) {
		fprintf(stderr,"  %d\n",rc);
	}
	exit(rc);
}

