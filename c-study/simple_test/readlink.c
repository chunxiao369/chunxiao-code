#include <stdio.h>
#include <unistd.h>
#define MAXSIZE 1024
int main()
{
	char buf[MAXSIZE];
	printf("This is a test!\n");
	readlink("/proc/self/exe",buf,MAXSIZE);
	printf("%s",buf);
}

