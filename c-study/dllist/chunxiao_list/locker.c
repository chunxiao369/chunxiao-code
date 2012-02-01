#include "locker.h"

comm_value_t locker_loop_lock(locker_t * thiz)
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

comm_value_t locker_loop_unlock(locker_t * thiz)
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

void locker_loop_destroy(locker_t * thiz)
{
    return_if_fail(thiz != NULL && thiz->destroy != NULL);

    thiz->owner = 0;
    thiz->refcount = 0;
    if (thiz->real_destroy) {
        thiz->real_destroy(thiz);
    }

    SAFE_FREE(thiz);

    return;
}

locker_t* locker_loop_create(task_self_func task_self,
                             locker_lock_func real_lock,
                             locker_unlock_func real_unlock,
                             locker_destroy_func real_destroy,
                             locket_init_func init,
                             int data_len)
{
    locker_t* thiz = NULL;
    return_val_if_fail(task_self != NULL, NULL);
    return_val_if_fail(real_lock != NULL, NULL);
    return_val_if_fail(real_unlock != NULL, NULL);
    return_val_if_fail(real_destroy != NULL, NULL);

    thiz = (locker_t *)malloc(sizeof(locker_t) + data_len);

    if(thiz != NULL) {
        thiz->lock    = locker_loop_lock;
        thiz->unlock  = locker_loop_unlock;
        thiz->destroy = locker_loop_destroy;

        thiz->real_lock    = real_lock;
        thiz->real_unlock  = real_unlock;
        thiz->real_destroy = real_destroy;

        thiz->owner = 0;
        thiz->refcount = 0;
        thiz->task_self = task_self;
    }
    init(thiz);

    return thiz;
}

