
#if defined(__KERNEL__) && defined(linux)
#include <linux/module.h>
#include <linux/kernel.h>
#define printf printk
#else
#include <stdio.h>
#endif
#include <stdlib.h>

struct rte_flow {
	int a;
};

struct rte_flow *rte_flow_createA(void)
{
	struct rte_flow *f;
	f = (struct rte_flow *)malloc(sizeof(struct rte_flow));
	f->a = 1;
	return f;
}

void rte_flow_printA(struct rte_flow *f)
{
    printf("AAA: %d.\n", f->a);
}

int ta(void)
{
	int a = 100;
	printf("in ta, a: %d\n", a);
	return 0;
}
