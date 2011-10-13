#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#if 0
int my_printf   (const   char   *format,   ...) 
{ 
    va_list  arg; 
    int   done; 
    va_start   (arg,   format); 
    done   =   vprintf(format,   arg);
    va_end   (arg); 
    return   done; 
}
#endif

#if 0
#define  my_printf(format, args...)      { \
   printf(format, ##args);                \
}
#endif

#if 0
int my_printf(const   char   *format, args...) 
{
    return printf(format, #args);
}
#endif
int my_printf(const char *fmt,...)
{
    va_list ap;
    char string[256];
    int   done; 
    va_start(ap,fmt);
    vsprintf(string, fmt, ap);
    done = printf("%s", string);
    va_end(ap);
    return done;
}

int main()
{
    int a = 2;
    my_printf("sssssssss%d\n", 2);
}

