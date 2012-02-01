/**
 * locker_pthread.h
 * xuchunxiao369@gmail.com
 * Copyright (C) <1983-2011>
 * locker interface 

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOCKER_PTHREAD_H
#define LOCKER_PTHREAD_H
#include "locker.h"
typedef struct _PrivInfo {
    pthread_mutex_t mutex;
} PrivInfo;
comm_value_t locker_pthread_lock(locker_t * thiz);
comm_value_t locker_pthread_unlock(locker_t * thiz);
void locker_pthread_destroy(locker_t * thiz);
void locker_pthread_init(locker_t *thiz);
int locket_pthread_self(void);
#endif /*LOCKER_PTHREAD_H */
