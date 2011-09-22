#include <stdio.h>
#include "app.h"
#include "common.h" 

int a_func()
{
    printf("xuchunxiao a_func.\n");
	return 0;
}

int b_func()
{
    printf("xuchunxiao b_func.\n");
	return 0;
}

int c_func()
{
    printf("xuchunxiao c_func.\n");
	return 0;
}

int main()
{
	obj_t test_obj;

    test_obj.id = 0;
    test_obj.type = TYPE_A_MODULE;

    printf("in main\n");
	init_module();

    module_register(TYPE_A_MODULE, 0, a_func);
    module_register(TYPE_B_MODULE, 0, b_func);
    module_register(TYPE_C_MODULE, 0, c_func);
	
    while (1)
    {
        if (module_do(&test_obj))
        {
            printf("module_do error!\n");
            break;
        }
        test_obj.type++;
    }
    tb();

	return 0;
}
