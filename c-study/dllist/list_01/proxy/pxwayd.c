#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>

#define EOT 4
#define ENQ 5
#define MARK "/var/run/noconf"


const char *skaddr="/tmp/mysocket";
const char logoend[5]={ EOT,0,0,0,'\n'};
char buf[1024];

void output(int fd )
{
	int  n;

	while (fgets(buf, sizeof(buf), stdin)) {
		//usleep(100);
		write( fd, buf, strlen(buf) );
		if (*buf==EOT) { break; }
		if (*buf==ENQ) { 
			n=read( fd,buf,sizeof(buf));
			write( 1, buf, n );
		} 
	}
}

void fdcloseall(int sd,int *client,int size)
{
	int i;
	for (i=0;i<size;i++) if (client[i]>0) close(client[i]);
	close(sd);
}


int main(int argc, char **argv)
{
	int fd1[2], fd2[2];
	fd_set fdR, set;
	int s, fd, nread, i, n, len, ts, client[FD_SETSIZE];
	int maxi = 0, maxfd = 0;
	pid_t pid;
	struct sockaddr_un saun, fsaun;
	struct timeval timeout;
	char logo[1024];


	if (argc != 2) {
		fprintf(stderr, "Usage: %s path/prog\n",
			basename(argv[0]));
		exit(1);
	}

	if ( access(argv[1],X_OK)< 0 ) { 
		fprintf(stderr, "%s can not exec\n", 
			argv[1]);
		exit(1);
	}

	daemon(1,1);

	signal(SIGCLD, SIG_IGN);
	signal(SIGPIPE,SIG_IGN);

	if (pipe(fd1) < 0 || pipe(fd2) < 0) {
		perror("pipe error");
		exit(1);
	}
	
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	if (pid == 0) {		/* CHILD */
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) {
				perror("dup2 error to stdin");
				exit(1);
			}
			close(fd1[0]);
		}
		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) {
				perror("dup2 error to stdout");
				exit(1);
			}
			close(fd2[1]);
		}
		if (execl(argv[1], basename(argv[1]), (char *) 0) < 0) {
			perror("execl error");
			exit(1);
		}
	}
	close(fd1[0]);
	close(fd2[1]);

	sleep(2);

	dup2(fd2[0],0);
	dup2(fd1[1],1);

	logo[0]=0;
	while(fgets(buf,1024,stdin)){ 
		if(*buf==EOT) { 
			break;
		}
		if(strlen(buf)+strlen(logo)>sizeof(logo)) logo[0]=0; 
		strcat(logo,buf);
	}


	unlink(skaddr);
	for (i = 0; i < FD_SETSIZE; i++) {
		client[i] = -1;
	}

	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Cannot create socket");
		exit(1);
	}

	saun.sun_family = AF_UNIX;
	strcpy(saun.sun_path, skaddr);

	if (bind(s, (struct sockaddr *) &saun, sizeof(saun)) < 0) {
		perror("Cannot name the socket");
		exit(1);
	}

	if (listen(s, 2) < 0) {
		perror("Can not create the socket queue");
		exit(1);
	}


	FD_ZERO(&set);
	FD_SET(s, &set);
	len = sizeof(fsaun);
	maxfd = s;

	while (1) {
		fdR = set;
		timeout.tv_sec = 100;
		timeout.tv_usec = 0;

		nread = select(maxfd + 1, &fdR, NULL, NULL, &timeout);
		if (nread < 1) {
			continue;
		}

		if (FD_ISSET(s, &fdR)) {
			fd = accept(s, (struct sockaddr *) &fsaun, &len);

			write(fd, logo, strlen(logo));
			write(fd, logoend,sizeof(logoend));

			for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = fd;
					break;
				}
			}
			if (i == FD_SETSIZE) {
				fprintf(stderr, "too many clients");
				exit(0);
			}
			FD_SET(fd, &set);
			if (fd > maxfd)
				maxfd = fd;	/* for select */
			if (i > maxi)
				maxi = i;
			if (--nread <= 0)
				continue;
		}
		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ((fd = client[i]) < 0)
				continue;
			if (!FD_ISSET(fd, &fdR))
				continue;

			if ((n = read(fd, buf, 1024)) <= 0) {
				close(fd);
				FD_CLR(fd, &set);
				client[i] = -1;
			} else {
				buf[n]=0;
				write(1, buf, n);
				output(fd);
			}
			if (--nread <= 0)
				break;
		}
	}
	exit(0);
}

