#include <stdio.h>
#include <string.h>
#include "app.h" 
#include "common.h" 


module_t module_a[MAX_A_MODULE];
module_t module_b[MAX_B_MODULE];
module_t module_c[MAX_C_MODULE];

int module_register(int type, int id, func_t p)
{
     
    if (type == TYPE_A_MODULE)
    {
        module_a[id].func = p;
    }
    else if (type == TYPE_B_MODULE)
    {
        module_b[id].func = p;
    }
    else if (type == TYPE_C_MODULE)
    {
        module_c[id].func = p;
    }

    return 0;
}

int module_do(obj_t *obj)
{
    int id;
    func_t func_p = NULL;

    id = obj->id;
    if (obj->type == TYPE_A_MODULE)
    {
        func_p = module_a[id].func;
    }
    else if (obj->type == TYPE_B_MODULE)
    {
        func_p = module_b[id].func;
    }
    else if (obj->type == TYPE_C_MODULE)
    {
        func_p = module_c[id].func;
    }

    if (NULL == func_p)
        return 1;
    func_p();

	return 0;
}

int init_module()
{
	printf("init module\n");
    memset(module_a, 0, sizeof(module_t) * MAX_A_MODULE);
    memset(module_b, 0, sizeof(module_t) * MAX_B_MODULE);
    memset(module_c, 0, sizeof(module_t) * MAX_C_MODULE);
	return 0;
}


