
#if defined(__KERNEL__) && defined(linux)
#include <linux/module.h>
#include <linux/kernel.h>
#define printf printk
#else
#include <stdio.h>
#endif
#include "app.h"

int ta(void)
{
	printf("in ta, a: %d\n", a);
	return 0;
}
