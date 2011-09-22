#include "list.h"
int test() 
{
	printf("\nTest!\n\n");
}

int init_list(List * list) 
{
	(*list) = (List) malloc(sizeof(struct node_t));
	if ((*list) == NULL) {
		printf("InitList error!\n");
		return 0;
	} else {
		(*list)->next = NULL;
		(*list)->prev = NULL;
		return 1;
	}
}
int destroy_list(List * list) 
{
	List q, p;
	if (*list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	}
	p = (*list)->next;
	while (p != NULL){
		q = p->next;
		free(p->element);
		free(p);
		p = q;
	}
	free(*list);
	*list = NULL;
	return 1;
}

int clear_list(List list) 
{
	List q, p;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	}
	p = list->next;
	while (p != NULL) {
		q = p->next;
		free(p);
		p = q;
	}
	list->next = NULL;
	list->prev = NULL;
	return 1;
}

int list_empty(List list) 
{
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	} else if (list->next == NULL && list->prev == NULL)
		return 1;
	
	else
		return 0;
}

int list_length(List list) 
{
	int i = 0;
	List p;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	}
	p = list->next;
	while (p != NULL) {
		i++;
		p = p->next;
	}
	return i;
}
int get_element(List list, int i, element_type * e) 
{
	int j = 0;
	List p;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	}
	p = list->next;
	if (p == NULL) {
		printf("The dllist is null!\n");
		return -1;
	}
	if (i < 1 || i > list_length(list)) {
		printf("The i is wrong\n");
		return -1;
	}
	while (p != NULL && j != i)
		j++;
	*e = p->element;
	return 1;
}

int locate_element(List list, element_type e,
		     int (*compare) (element_type, element_type)) 
{
	int i = 0;
	List p;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	}
	p = list->next;
	while (p != NULL) {
		i++;
		if (compare(p->element, e))
			return i;
		p = p->next;
	}
	return 0;
}


int prior_element(List list, element_type cure, element_type * preve) 
{
	List p;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	} else if (list->next == NULL) {
		printf("The dllist is NULL\n");
		return -1;
	}
	p = list->next->next;
	while (p != NULL) {
		if (p->element == cure) {
			*preve = p->prev->element;
			return 1;
		}
		//Here may be has a bug!
		//return 0;
	}
	return 0;
}
int next_element(List list, element_type cure, element_type * nexte) 
{
	List p;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	} else if (list->next == NULL) {
		printf("The dllist is NULL\n");
	}
	p = list->next->next;
	while (p != NULL) {
		if (p->prev->element == cure)
		{
			*nexte = p->next;
			return 1;
		}
		p = p->next;
		}
	return 0;
}

ptr_to_node get_elem_node(List list, int i)
{
	int j;
	List p = list;
	if (list == NULL) {
		printf("The dllist is not exist!\n");
		return NULL;
	}
	if (i < 1 || i > list_length(list)) {
		printf("The parameter \'i\' is over the border!\n");
		return NULL;
	}
	for (j = 1; j <= i; j++)
		p = p->next;
	return p;
}

int list_insert(List * list, int i, element_type e) 
{
	List p, s;
	if (*list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	}
	if (i < 0 || i > list_length(*list) + 1) {
		printf("The parameter \'i\' is over the border!\n");
		return -1;
	}
	s = (List) malloc(sizeof(struct node_t));
	if (!s) {
		return 0;
	}
	s->element = e;	//Warning!!!
	
	//printf("InSert %d now\n",*((int *)s->element));
	if ((*list)->next == NULL) {
		s->prev = *list;
		s->next = NULL;
		(*list)->next = s;
	} else if (i == 0) {
		s->next = (*list)->next;
		s->prev = (*list);
		(*list)->next->prev = s;
		(*list)->next = s;
	} else if (i == list_length((*list))) {
		p = get_elem_node((*list), i);
		s->next = NULL;
		s->prev = p;
		p->next = s;
	} else {
		p = get_elem_node((*list), i);
		s->prev = p;
		s->next = p->next;
		p->next->prev = s;
		p->next = s;
	}
	return 1;
}

int list_delete(List * list, int i, element_type * e) 
{
	List p;
	int len;
	if (*list == NULL) {
		printf("The dllist is not exist!\n");
		return -1;
	} else if (i < 1 || i > list_length(*list)) {
		printf("The parameter \'i\' is over the border!\n");
		return -1;
	}
	p = (*list)->next;
	if (!p) {
		printf("The dllist is NULL!\n");
		return 0;
	}
	len = list_length(*list);
	if (i == list_length(*list)) {
		p = get_elem_node(*list, len);
		if (!p)
			return 0;
		p->prev->next = NULL;
		*e = p->element;
		
		//printf("delete-e:%d\n",*((int *)p->element));
		free(p);
	} else {
		p = get_elem_node(*list, i);
		printf("delete the %d\n", i);
		if (!p)
			return 0;
		//*e = p->element;
		//printf("delete-e:%d\n",*((int *)p->element));
		p->prev->next = p->next;
		p->next->prev = p->prev;
		*e = p->element;
		free(p);
	}
	return 1;
}
