/**
 * ret_value.h
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

#ifndef __RET_VALUE__
#define __RET_VALUE__
typedef enum _comm_value {
    RET_OK,
    RET_NOMEM,
    RET_STOP,
    RET_INVALID_PARAMS,
    RET_FAIL
} comm_value_t;
#endif
