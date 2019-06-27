#include <stdio.h>

typedef struct _em_list {
     struct _em_list *next;
     int v;
} em_list_t;

int em_list_print(em_list_t *head)
{
    while (head) {
        printf ("value: %d\n", head->v);
        head = head->next;
    }
    printf("\n");
    return 0;
}

int list_reverse(em_list_t *head, em_list_t **newl)
{
    em_list_t *new = NULL;
    em_list_t *old, *next;
    old = head;
    while (old) {
        next = old->next;
        old->next = new;
        new = old;
        old = next;
    }
    *newl = new;
    return 0;
}

int main(int argc, char **argv)
{
    em_list_t *head;
    em_list_t *head_n;

    em_list_t node1;
    em_list_t node2;
    em_list_t node3;
    em_list_t node4;
    em_list_t node5;
    em_list_t node6;

    node1.next = &node2;
    node2.next = &node3;
    node3.next = &node4;
    node4.next = &node5;
    node5.next = &node6;
    node6.next = NULL;

    node1.v    = 1;
    node2.v    = 2;
    node3.v    = 3;
    node4.v    = 4;
    node5.v    = 5;
    node6.v    = 6;

    head = &node1;
    em_list_print(head);
    list_reverse(head, &head_n);
    em_list_print(head_n);
    return 0;
}
