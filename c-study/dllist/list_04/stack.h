#ifndef __STACK_H__
#define __STACK_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#define stack_about list_head_t

typedef struct _stack {
    list_head_t stack_list;
} stack_t;

static void stack_init(stack_t *s)
{
    LIST_HEAD_INIT(&(s->stack_list));
}

void stack_push(stack_about *node, stack_t *s)
{
    list_add(node, &(s->stack_list));
}

static inline int stack_empty(const stack_t *s)
{
    return list_empty(&(s->stack_list));
}

static inline void * stack_pop(stack_t *s, char *type, char *member)
{
}
#if 0
static inline void * stack_pop(stack_ptr, type, member)
{
    struct list_head *i_list;
    type *data = NULL;
    list_for_each(i_list, &((stack_ptr)->stack_list)) {
    data = list_entry(i_list, type, member);
    list_del(&(data->member));
    break;
    }
    data;
}
#endif
#define stack_pop(stack_ptr, type, member) ({           \
    struct list_head *i_list;                           \
    type *data = NULL;                                  \
    int ret = 1;                                        \
    list_for_each(i_list, &((stack_ptr)->stack_list)) { \
    data = list_entry(i_list, type, member);            \
    list_del(&(data->member));                          \
    ret = 0;                                            \
    break;                                              \
    }                                                   \
    ret;
})

#define stack_top(stack_ptr, type, member) ({           \
    struct list_head *i_list;                           \
    type *data = NULL;                                  \
    list_for_each(i_list, &((stack_ptr)->stack_list)) { \
    data = list_entry(i_list, type, member);            \
    break;                                              \
    }                                                   \
    data;                                               \
})

#endif
