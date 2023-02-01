#include <stdio.h>
#include "app.h"

struct rte_flow *rte_flow_create(int type)
{
    if (type == 1) {
        return rte_flow_createA();
    }
    return rte_flow_createB();
}

void rte_flow_print(int type, struct rte_flow *f)
{
    if (type == 1) {
        return rte_flow_printA(f);
    }
    return rte_flow_printB(f);
}

int main()
{
	struct rte_flow *f;
	printf("in main\n");
	ta();
	tb();
	f = rte_flow_create(2);
	rte_flow_print(2, f);
	return 0;
}
