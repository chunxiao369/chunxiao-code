
#if defined(__KERNEL__) && defined(linux)
#include <linux/module.h>
#include <linux/kernel.h>
#define printf printk
#else
#include <stdio.h>
#endif

int ta(void)
{
	printf("in ta\n");
	return 0;
}
