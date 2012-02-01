/**
 * locker.h
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
#ifndef __LOCKER_H__
#define __LOCKER_H__

#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "ret_value.h"
#include "ofen_used.h"

struct _locker;
typedef struct _locker locker_t;

typedef comm_value_t(*locker_lock_func) (locker_t * thiz);
typedef comm_value_t(*locker_unlock_func) (locker_t * thiz);
typedef void (*locker_destroy_func) (locker_t * thiz);
typedef int (*task_self_func) (void);
typedef void (*locket_init_func) (locker_t * thiz);

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
    char priv[0];
};

comm_value_t locker_loop_lock(locker_t * thiz);
comm_value_t locker_loop_unlock(locker_t * thiz);
void locker_loop_destroy(locker_t * thiz);

locker_t* locker_loop_create(task_self_func task_self,
                      locker_lock_func real_lock,
                      locker_unlock_func real_unlock,
                      locker_destroy_func real_destroy,
                      locket_init_func init,
                      int data_len);

#endif
