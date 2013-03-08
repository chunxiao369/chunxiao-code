
#if defined(__KERNEL__) && defined(linux)
#include <linux/module.h>
#include <linux/kernel.h>
#define printf printk
#else
#include <stdio.h>
#endif

int tb(void)
{
	printf("in tb\n");
	return 0;
}

