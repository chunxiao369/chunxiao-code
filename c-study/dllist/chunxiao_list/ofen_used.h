/**
 * often_used.h
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

#ifndef __OFEN_USED_H__
#define __OFEN_USED_H__

#include <stdio.h>

#define return_if_fail(p) if(!(p)) \
    {printf("%s:%d Warning: "#p" failed.\n", \
        __func__, __LINE__); return;}

#define return_val_if_fail(p, ret) if(!(p)) \
    {printf("%s:%d Warning: "#p" failed.\n",\
    __func__, __LINE__); return (ret);}

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

#define print_data(_ptr, _len) {                     \
    unsigned char *ptr, int len;                     \
    ptr = _ptr;                                      \
    len = _len;                                      \
    int i;                                           \
    printf("ptr start at %p, len: %d.\n", ptr, len); \
    for (i = 0; i < len; i++) {                      \
        if ((i % 16 == 0) && (i != 0))               \
            printf("\n");                            \
        printf("%02x ", ptr[i]);                     \
    }                                                \
    printf("\n");                                    \
}

#endif
