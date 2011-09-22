#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
/*#define NULL 0*/
typedef void *element_type;

struct node_t;
typedef struct node_t *ptr_to_node;
typedef ptr_to_node List;
/*
typedef PtrToNode Position;
*/

struct node_t {
	element_type element;
	List prev;
	List next;
};

int test();

int init_list(List * list);
int destroy_list(List * list);
int clear_list(List list);
int list_empty(List list);
int list_length(List list);
int get_element(List list, int i, element_type * e);
int locate_element(List list, element_type e,
		   int (*compare) (element_type, element_type));
int prior_element(List list, element_type cure, element_type * preve);
int next_element(List list, element_type cure, element_type * nexte);
ptr_to_node get_elem_node(List list, int i);
int list_insert(List * list, int i, element_type e);
int list_delete(List * list, int i, element_type * e);
