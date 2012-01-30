#ifndef __LOCKER_H__
#define __LOCKER_H__

struct _locker;
typedef struct _locker locker_t;

typedef comm_value_t(*locker_lock_func) (locker_t * thiz);
typedef comm_value_t(*locker_unlock_func) (locker_t * thiz);
typedef void (*locker_destroy_func) (locker_t * thiz);
typedef int (*task_self_func) (void);

struct _locker {
    int owner;
    int refcount;
    task_self_func task_self;
    locker_lock_func lock;
    locker_unlock_func unlock;
    locker_destroy_func destroy;
    locker_lock_func real_lock;
    locker_unlock_func real_unlock;
    locker_destroy_func real_destroy;
};

static comm_value_t locker_loop_lock(locker_t * thiz)
{
    comm_value_t ret = RET_OK;

    return_val_if_fail(thiz != NULL && thiz->lock != NULL, RET_INVALID_PARAMS);
    if (thiz->owner == thiz->task_self()) {
        thiz->refcount++;
    } else {
        if ((ret = thiz->real_lock(thiz)) == RET_OK) {
            thiz->refcount = 1;
            thiz->owner = thiz->task_self();
        }
    }

    return ret;
}

static comm_value_t locker_loop_unlock(locker_t * thiz)
{
    comm_value_t ret = RET_OK;

    return_val_if_fail(thiz != NULL && thiz->real_unlock != NULL, RET_INVALID_PARAMS);
    return_val_if_fail(thiz->owner == thiz->task_self(), RET_FAIL);

    thiz->refcount--;
    if (thiz->refcount == 0) {
        thiz->owner = 0;
        ret = thiz->real_unlock(thiz);
    }

    return ret;
}

static void locker_loop_destroy(locker_t * thiz)
{
    return_if_fail(thiz != NULL && thiz->destroy != NULL);

    thiz->owner = 0;
    thiz->refcount = 0;
    thiz->real_destroy(thiz);
    priv->real_locker = NULL;

    SAFE_FREE(thiz);

    return;
}

locker_t* locker_loop_create(task_self_func task_self,
                             locker_lock_func real_lock,
                             locker_unlock_func real_unlock,
                             locker_destroy_func real_destroy)
{
    locker_t* thiz = NULL;
    return_val_if_fail(task_self != NULL, NULL);
    return_val_if_fail(real_lock != NULL, NULL);
    return_val_if_fail(real_unlock != NULL, NULL);
    return_val_if_fail(real_destroy != NULL, NULL);

    thiz = (locker_t *)malloc(sizeof(locker_t));

    if(thiz != NULL) {
        thiz->lock    = real_lock;
        thiz->unlock  = real_unlock;
        thiz->destroy = real_destroy;

        thiz->owner = 0;
        thiz->refcount = 0;
        thiz->task_self = task_self;
    }

    return thiz;
}

#endif
