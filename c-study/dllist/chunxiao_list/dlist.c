#include "dlist.h"
list_t *list_create(data_destroy_func data_destroy, void *context, locker_t * lock)
{
    list_t *new_list = NULL;
    new_list = malloc(sizeof(list_t));
    if (new_list != NULL) {
        new_list->head.next = &new_list->head;
        new_list->head.prev = &new_list->head;
        new_list->locker = lock;
        new_list->data_destroy_ctx = context;
        new_list->data_destroy = data_destroy;
    }
    return new_list;
}

list_node_t *node_create(void *data)
{
    list_node_t *node;
    node = malloc(sizeof(list_node_t));
    node->data = data;
    return node;
}

static inline void __list_add(struct _list_node *_new, struct _list_node *prev, struct _list_node *next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

static void dlist_lock(list_t * thiz)
{
    if (thiz->locker != NULL) {
        thiz->locker->lock(thiz->locker);
    }

    return;
}

static void dlist_unlock(list_t * thiz)
{
    if (thiz->locker != NULL) {
        thiz->locker->unlock(thiz->locker);
    }

    return;
}

int list_add(list_t * list, void *data)
{
    list_node_t *node = NULL;
    return_val_if_fail(list != NULL, RET_INVALID_PARAMS);
    node = node_create(data);
    return_val_if_fail(node != NULL, RET_NOMEM);
    dlist_lock(list);
    __list_add(node, &list->head, list->head.next);
    dlist_unlock(list);
    return 0;
}

int list_add_tail(list_t * list, void *data)
{
    list_node_t *node = NULL;
    return_val_if_fail(list != NULL, RET_INVALID_PARAMS);
    node = node_create(data);
    return_val_if_fail(node != NULL, RET_NOMEM);
    dlist_lock(list);
    __list_add(node, list->head.prev, &list->head);
    dlist_unlock(list);
    return 0;
}

static void list_destroy_data(list_t * list, void *data)
{
    if (list->data_destroy != NULL) {
        list->data_destroy(list->data_destroy_ctx, data);
    }
    return;
}

static void list_destroy_node(list_t * list, list_node_t * node)
{
    if (node != NULL) {
        node->next = NULL;
        node->prev = NULL;
        list_destroy_data(list, node->data);
        SAFE_FREE(node);
    }
    return;
}

static inline void __list_del(list_node_t * prev, list_node_t * next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_delete(list_node_t * entry)
{
    __list_del(entry->prev, entry->next);
}

static inline void list_del_and_destroy_node(list_t * list, list_node_t * entry)
{
    list_delete(entry);
    list_destroy_node(list, entry);
}

static void dlist_destroy_locker(list_t * thiz)
{
    if (thiz->locker != NULL) {
        locker_loop_unlock(thiz->locker);
        locker_loop_destroy(thiz->locker);
        thiz->locker = NULL;
    }

    return;
}

void list_destroy(list_t * new_list)
{
    list_node_t *iter = NULL;
    list_node_t *next = NULL;

    return_if_fail(new_list != NULL);

    dlist_lock(new_list);

    iter = new_list->head.next;
    while (iter != &new_list->head) {
        next = iter->next;
        list_destroy_node(new_list, iter);
        iter = next;
    }

    new_list->head.next = &new_list->head;
    new_list->head.prev = &new_list->head;
    dlist_destroy_locker(new_list);

    SAFE_FREE(new_list);
    return;
}

size_t list_length(list_t * thiz)
{
    size_t length = 0;
    list_node_t *iter = NULL;

    return_val_if_fail(thiz != NULL, 0);
    dlist_lock(thiz);
    iter = &thiz->head;
    while (iter->next != &thiz->head) {
        length++;
        iter = iter->next;
    }
    dlist_unlock(thiz);

    return length;
}
