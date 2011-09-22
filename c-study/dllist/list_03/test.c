#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#define DEBUG do {                                 \
       printf("**************************\n");        \
       printf("list 1:\n");                         \
       list_for_each(i, &list1) {              \
              mylist_t *ops =            \
              list_entry(i,                         \
              mylist_t, list);         \
              printf("pf is %d\n", ops->pf);              \
       }                                        \
                                                 \
       printf("list 2:\n");                         \
       list_for_each(i, &list2) {              \
              mylist_t *ops =            \
              list_entry(i,                         \
              mylist_t, list);         \
              printf("pf is %d\n", ops->pf);              \
       }                                        \
} while(0);

#if 1
typedef struct __mylist {
       int pf;
       char name[FILENAME_MAX];
       struct list_head list;
} mylist_t;
#else
typedef struct __mylist {
       struct list_head list;
       int pf;
       char name[FILENAME_MAX];
} mylist_t;
#endif
static LIST_HEAD(list1);             /* define 2 list */
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
    list_add(&elem1.list, &list1);
    //list_del(&elem1.list);

    list_add(&elem2.list, &list1);
    //list_add_tail(&elem2.list, &list1);
    list_add_tail(&elem3.list, &list1);
    list_for_each(i_list, &list1) {
        mylist_t *ops = list_entry(i_list, mylist_t, list);
        printf("pf is %d\n", ops->pf);
    }

#if 0
    /* move element to another list */
    list_move_tail(&elem2.list, &list2);
    /* merge */
    list_splice_init(&list1, &list2);
    DEBUG;
#endif
    return 0;
}


