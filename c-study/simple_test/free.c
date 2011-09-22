#include "stdio.h"
#include "string.h"
#include <stdlib.h>
int *p2;
int a;
struct abc {
    int a;
    int b;
    int c;
    };
struct abc *dev = NULL;
int func(void **abc)
{
    abc = abc;
    
    return 0;
}
int main()
{
    char *p;
    char a[2] = {'a', 'b'};
    char *line_new;
    p=malloc(4);
    free(p);
    line_new = strdup( "create queue all" );
    printf("%s\n",line_new);
    p = a;
    printf("%s", p);
    return 0;

    
    p2 = &a;

    func((void **)&dev);
    return 0;
    //free(p);
}
