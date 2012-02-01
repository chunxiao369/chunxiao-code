
#include <stdlib.h>
#include <pthread.h>
#include "locker_pthread.h"


comm_value_t locker_pthread_lock(locker_t * thiz)
{
    PrivInfo *priv = (PrivInfo *) thiz->priv;

    int ret = pthread_mutex_lock(&priv->mutex);

    return ret == 0 ? RET_OK : RET_FAIL;
}

comm_value_t locker_pthread_unlock(locker_t * thiz)
{
    PrivInfo *priv = (PrivInfo *) thiz->priv;

    int ret = pthread_mutex_unlock(&priv->mutex);

    return ret == 0 ? RET_OK : RET_FAIL;
}

void locker_pthread_destroy(locker_t * thiz)
{
    PrivInfo *priv = (PrivInfo *) thiz->priv;

    pthread_mutex_destroy(&priv->mutex);

    //SAFE_FREE(thiz);

    return;
}

void locker_pthread_init(locker_t *thiz)
{
    if (thiz != NULL) {
        PrivInfo *priv = (PrivInfo *) thiz->priv;
        pthread_mutex_init(&(priv->mutex), NULL);
    }
}

int locket_pthread_self(void)
{
    return pthread_self();
}

