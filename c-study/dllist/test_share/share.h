#ifndef __TEST_SHARE_H__
#define __TEST_SHARE_H__
#include "list.h"
typedef int (*ewx_test_func_t) (void);
int ewx_test_register(ewx_test_func_t f);
int ewx_test_init(void);
int ewx_test_destroy(void);
#endif
