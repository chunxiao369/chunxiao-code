#include<stdio.h>
#include<stdlib.h>

int a;
int b = 1;

int main(void)
{
    int n = 0;
    char *p1 = NULL;
    char *p2 = NULL;
    const int s = 10;
    p1 = (char *)malloc(200);
    p2 = "hello";

    printf("main  %p\n", main);
    printf("未初始化 a   %p\n", &a);
    printf("初始化   b   %p\n", &b);
    printf("局部变量 n   %p\n", &n);
    printf("动态内存 p1  %p\n", p1);
    printf("常量     s   %p\n", &s);
    printf("常字符串 p2  %p\n", p2);

    while (n < 100) {
        p1 = (char *)malloc(200);
	if ((n % 2) == 0) {
	    free(p1);
	}
	usleep(1000000);
	n++;
    }
}
