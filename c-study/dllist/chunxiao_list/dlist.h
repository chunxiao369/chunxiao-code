/**
 * dlist.h
 * xuchunxiao369@gmail.com
 * Copyright (C) <1983-2011>
 * double list

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
#ifndef __LIST_H__
#define __LIST_H__

#include "ret_value.h"
#include "locker.h"
#include "ofen_used.h"

typedef void (*data_destroy_func) (void *ctx, void *data);
typedef int (*data_compare_func) (void *ctx, void *data);
typedef comm_value_t(*data_visit_func) (void *ctx, void *data);

typedef struct _list_node {
    struct _list_node *prev;
    struct _list_node *next;
    void *data;
} list_node_t;

typedef struct _list {
    list_node_t head;
    locker_t *locker;
    void *data_destroy_ctx;
    data_destroy_func data_destroy;
} list_t;

list_t *list_create(data_destroy_func data_destroy, void *context, locker_t * lock);
void list_destroy(list_t * new_list);
int list_add(list_t * list, void *data);
int list_add_tail(list_t * list, void *data);
void list_destroy(list_t * new_list);
size_t list_length(list_t * thiz);

#endif
