
#include "log4c.h"

int main()
{
    int i = 0;
    if (log4c_init()) {
        printf("aaaaaaaaaaa");
        return 0;
    }
    log4c_category_t* mycat = log4c_category_get("octeon_cat");
    printf("mycat %p.\n", mycat);
    while (1) {
        log4c_category_log(mycat, LOG4C_PRIORITY_DEBUG, "Hello World!, %d", i);
        i++;
        if (i >= 1000)
            break;
    }
    return 0;
}

