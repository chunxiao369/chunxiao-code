#include <stdlib.h>
#include "list.h"
#include "share.h"

typedef struct _ewx_test_ {
    ewx_test_func_t init;
    list_head_t list;
} ewx_test_t;

static LIST_HEAD(g_test_list);

int ewx_test_register(ewx_test_func_t f)
{
    ewx_test_t *p = (ewx_test_t *)malloc(sizeof(ewx_test_t));
    if (p == NULL) {
        return -1;
    }
    p->init = f;
    list_add_tail(&p->list, &g_test_list);
    return 0;
}

int ewx_test_init(void)
{
    struct list_head *i;
    list_for_each(i, &g_test_list) {
        ewx_test_t *p = list_entry(i, ewx_test_t, list);
        p->init();
    }
    return 0;
}

int ewx_test_destroy(void)
{
    struct list_head *i;
    list_for_each(i, &g_test_list) {
        ewx_test_t *p = list_entry(i, ewx_test_t, list);
        list_del(&p->list);
        free(p);
    }

    return 0;
}
