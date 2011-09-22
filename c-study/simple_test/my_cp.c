#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<dirent.h>

#define BUFFSIZE 4096
/*
 * no buffer I/O, read write for example.
 * 
 * standard I/O, fgets, fgetc, fputc, getc, putc
 */

int main(int argc, char *argv[])
{
	int n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n) {
		    printf("usage: ls directory_name.\n ");
		    exit(1);
	    }
    if (n < 0)
        printf("read error!\n");

	exit(0);
}

