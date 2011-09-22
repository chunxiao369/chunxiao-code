
typedef int (*func_t)(void);

typedef struct _module {
    int type;
    int id;
    int reserve;
    func_t func;
} module_t;

typedef struct _obj {
    int type;
    int id;
} obj_t;

#define MAX_A_MODULE 5
#define MAX_B_MODULE 5
#define MAX_C_MODULE 5

#define TYPE_A_MODULE 1
#define TYPE_B_MODULE 2
#define TYPE_C_MODULE 3
int module_do(obj_t *obj);
int init_module();
int module_register(int type, int id, func_t p);
