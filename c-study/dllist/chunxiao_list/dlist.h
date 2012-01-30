
typedef void (*data_destroy_func)(void* ctx, void* data);
typedef int  (*data_compare_func)(void* ctx, void* data);
typedef Ret  (*data_visit_func)(void* ctx, void* data);

typedef struct _list_node {
    struct _list_node* prev;
    struct _list_node* next;
    void* data;
} list_node_t;

typedef struct _list {
    list_node head;
    locker_t *lock;
    void* data_destroy_ctx;
    data_destroy_func data_destroy;
} list_t;

list_t *list_create(locker_t* lock)
{
    list_t *new_list = NULL;
    new_list = malloc(sizeof(list_t));
    if (new_list != NULL) {
        new_list->head.next = &new_list->head;
        new_list->head.prev = &new_list->head;
        new_list->lock = lock;
    }
    return new_list;
}

void list_destroy(list_t *new_list)
{
    
}

list_node_t *node_create()
{
    list_node_t *node;
    node = malloc(sizeof(list_node_t));
    return node;
}

static inline void __list_add(struct _list_node *_new,
                  struct _list_node *prev,
                  struct _list_node *next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

static void dlist_lock(dlist_t* thiz)
{
    if(thiz->locker != NULL)
    {
        thiz->locker->lock(thiz->locker);
    }

    return;
}

static void dlist_unlock(dlist_t* thiz)
{
    if(thiz->locker != NULL)
    {
        thiz->locker->unlock(thiz->locker);
    }

    return;
}

void list_add(list_t *list, void *data)
{
    list_node_t *node = NULL;
    return_val_if_fail(list != NULL, RET_INVALID_PARAM);
    node = node_create();
    return_val_if_fail(node != NULL, RET_NOMEM);
    dlist_lock(list);
    __list_add(node, &list->head, list->head.next); 
    dlist_unlock(list);
}

void list_add_tail(list_t *list, void *data)
{
    list_node_t *node = NULL;
    return_val_if_fail(list != NULL, RET_INVALID_PARAM);
    node = node_create();
    return_val_if_fail(node != NULL, RET_NOMEM);
    dlist_lock(list);
    __list_add(node, list->head.prev, &list->head); 
    dlist_unlock(list);
}
