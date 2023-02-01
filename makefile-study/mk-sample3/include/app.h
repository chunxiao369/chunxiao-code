#ifndef _APP_H
#define _APP_H

int a;
int ta();
int tb();
int tc();

struct rte_flow;
struct rte_flow *rte_flow_createA(void);
struct rte_flow *rte_flow_createB(void);
void rte_flow_printA(struct rte_flow *f);
void rte_flow_printB(struct rte_flow *f);

#endif
