#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <signal.h> 

#define BLOCK_DEPTH 5
#define __DEBUG     1

#ifdef __DEBUG
#define debug_print(fmt, args...)       \
    do{                           \
        printf(fmt, ##args);      \
    }while(0)
#else
#define debug_print(fmt, args...)
#endif

typedef struct _list
{
    struct _list *next;
    int used[BLOCK_DEPTH];
    int data[BLOCK_DEPTH];
    //...
} list_t;

void init_array_list(void **p, int size)
{
    list_t *list_p;
    
    list_p = malloc(size * sizeof(list_t));
    memset(list_p, 0, size * sizeof(list_t));
    debug_print("malloc %p\n", list_p);

    *p = (void *)list_p;    
}

void free_array_list(void *p)
{
    debug_print("free %p\n", p);
    free(p);
}

void *get_new_list()
{
    list_t *p;
    p = malloc(sizeof(list_t));
    memset(p, 0, sizeof(list_t));
    //debug_print("get new list %p\n", p);    
    return (void *)p; 
}

/*
 * @array_n: hash value
 * @data:    real data
 */
void show_elem(void *base_addr, int array_n)
{
    list_t *list_p;
    int i;

    list_p = (list_t *)base_addr + array_n;
    while (list_p)
    {
        for (i = 0; i < BLOCK_DEPTH; i++)
        {
            /* if there is unused position */
            if (0 != list_p->used[i])
            {
                printf("i is: %d, data is %d.\n", i, list_p->data[i]);
            }
            else 
                break;
            /* if position is used, continue */
        }
        printf("\n");
        if (BLOCK_DEPTH > i) 
            return;

        if (BLOCK_DEPTH == i && list_p->next != NULL)
            list_p = list_p->next;
        else 
            break;
    }

    return;
}
/*
 * @array_n: hash value
 * @data:    real data
 * @return:  have added before return 2
             failed return 1;
 *           success return 0;
 */
int add_elem(void *base_addr, int array_n, int data)
{
    list_t *list_p;
    int i;

    list_p = (list_t *)base_addr + array_n;
    while (list_p)
    {
        for (i = 0; i < BLOCK_DEPTH; i++)
        {
            /* if there is unused position */
            if (0 == list_p->used[i])
            {
                list_p->used[i] = 1;
                list_p->data[i] = data;
                break;
            }
            if (data == list_p->data[i] && 1 == list_p->used[i])
                return 2;
            /* if position is used, continue */
        }
        if (BLOCK_DEPTH > i) 
            return 0;

        if (BLOCK_DEPTH == i && list_p->next != NULL)
            list_p = list_p->next;
        else 
        {
            list_p->next = (list_t *)get_new_list();
            if (NULL == list_p->next)
            {
                debug_print("mem alloc failed!\n");
                return 1;
            }
            list_p = list_p->next;
            list_p->used[0] = 1;
            list_p->data[0] = data;
            return 0;
        }
    }

    return 1;
}

/*
 * @array_n: hash value
 * @data:    real data
 * 
 * return 
 * success 0; not found 1;
 */
int del_elem(void *base_addr, int array_n, int data)
{
    list_t *list_p;
    list_t *last_p = NULL; /* find last one ptr */
    list_t *del_p = NULL;  /* find the del ptr */
    list_t *temp_p = NULL;
    int last_n = 0;     /* find last one num */
    int del_n = 0;      /* find del num */
    int del_found = 0;
    int last_found = 0;
    int i = 0;
    
    /* find the del one & find the last one */
    list_p = (list_t *)base_addr + array_n;
    temp_p = list_p;
    while (list_p)
    {
        for (i = 0; i < BLOCK_DEPTH; i++)
        {
            if (data == list_p->data[i])
            {
                del_n = i;
                del_p = list_p;
                del_found = 1;
            }
            if (0 == list_p->used[i])
            {
                if ( i > 0 )
                {
                    last_n = i - 1;
                    last_p = list_p;
                }    
                else
                {
                    last_n = BLOCK_DEPTH - 1;
                    last_p = temp_p; 
                } 
                last_found = 1;
            }
            if (1 == last_found && 1 == del_found)
                break;
        }
        debug_print("i %d, del_n %d, del found %d, last_n %d, last_found %d.\n", i, del_n, del_found, last_n, last_found);
        if ( BLOCK_DEPTH > i)
            break;
        temp_p = list_p;
        list_p = list_p->next;
        if (list_p == NULL)
        {
            last_n = BLOCK_DEPTH - 1;
            last_p = temp_p;
            last_found = 1;
            break;
        }
    }

    debug_print("i %d, del_n %d, del found %d, last_n %d, last_found %d.\n", i, del_n, del_found, last_n, last_found);
    if (0 == del_found || 0 == last_found)
        return 1;
    /* replace the del one by the last one */
    del_p->data[del_n] = last_p->data[last_n];
    last_p->used[last_n] = 0;

    if (0 == last_n && last_p != ((list_t *)base_addr + array_n))
    {
        temp_p->next = NULL;
        free((char *)last_p);
    }
    return 0;    
}


int main( int arg, char * argv[] )
{
    void *base = NULL;
    int i;
    int rt_v = 0;

    init_array_list(&base, 2);
    for (i = 0; i < 40; i++)
    {
        rt_v = add_elem(base, 0, 255 - i);
        if (rt_v)
        {
            printf("add error!\n");
            return 1;
        }
    }

    for (i = 0; i < 40; i++)
    {
        rt_v = del_elem(base, 0, 255 - i);
        if (rt_v)
        {
            printf("del error!\n");
            return 1;
        }
    }
    show_elem(base, 0);
    free_array_list(base);
 
    return 0;
}


