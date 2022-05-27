
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "list.h"

list_head_t head;

struct aa_ {
    int a;
    int b;
    list_head_t list;
};
int main()
{
    struct aa_ a = {1, 2, NULL};
    struct aa_ b = {3, 4, NULL};

    LIST_HEAD_INIT(&head);
    return 0;
}

