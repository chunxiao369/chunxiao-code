#include <stdio.h>

//extern char **environ;
main ()
{
    int i = 0;
    close(1);
    printf("hello, world.\n");
    //for (i = 0; environ[i] != NULL; i++)
        //printf("environ %02d: %s.\n", i, environ[i]);
}

