
#if defined(__KERNEL__) && defined(linux)
#include <linux/module.h>
#include <linux/kernel.h>
#define printf printk
#else
#include <stdio.h>
#endif
#include <stdlib.h>

struct rte_flow {
	int b;
};
struct rte_flow *rte_flow_createB(void)
{
	struct rte_flow *f;
	f = (struct rte_flow *)malloc(sizeof(struct rte_flow));
	f->b = 1;
	return f;
}

void rte_flow_printB(struct rte_flow *f)
{
    printf("BBB: %d.\n", f->b);
}

int tb(void)
{
	printf("in tb\n");
	return 0;
}

