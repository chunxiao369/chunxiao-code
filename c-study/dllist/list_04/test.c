#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "stack.h"

#if 0
typedef struct __mylist {
       struct list_head list;
       int pf;
       char name[FILENAME_MAX];
} mylist_t;

static LIST_HEAD(list1);
static LIST_HEAD(list2);

int main(int argc, char *argv[])
{
    struct list_head *i_list;
    mylist_t elem1;
    mylist_t elem2;
    mylist_t elem3;

    LIST_HEAD_INIT(&list1);
    LIST_HEAD_INIT(&list2);
    elem1.pf = 2007;
    elem2.pf = 2008;
    elem3.pf = 2009;
    list_add(&elem3.list, &list1);
    list_add(&elem2.list, &list1);
    list_add(&elem1.list, &list1);
    list_for_each(i_list, &list1) {
        mylist_t *ops = list_entry(i_list, mylist_t, list);
        printf("pf is %d\n", ops->pf);
    }
    list_for_each_inverted(i_list, &list1) {
        mylist_t *ops = list_entry(i_list, mylist_t, list);
        printf("pf is %d\n", ops->pf);
    }

    return 0;
}
#endif

typedef struct _node {
       stack_about about;
       int pf;
} node_t;

int main(int argc, char *argv[])
{
    stack_t xu_stack;
    node_t *node = NULL;
    node_t node_2;
    node_2.pf = 24;

    stack_init(&xu_stack);
    stack_push(&node_2.about, &xu_stack);
    printf("is empty: %d.\n", stack_empty(&xu_stack));
    node = stack_pop(&xu_stack, node_t, about);
    printf("node: %p\n", node);
    if (node != NULL) {
        printf("node->pf: %d\n", node->pf);
    }

    return 0;
}
