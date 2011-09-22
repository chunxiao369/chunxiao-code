#include "stdio.h"
#include "unistd.h"

#define  pa_tap_print( format, args...)           printf( "  "format,##args)
#define  ppe_print(s, format, args...)      { \
   printf("  %-30s",s);                              \
   printf(format, ##args);                          \
}

#pragma message("aaaaaaaaaaaaa")
#define PRINT 4
int main()
{
    int i = 1;
    while (i <= 100) {
        usleep(1000000);
        printf("%03d%%\n", PRINT);
        if (i == 2) {
#undef PRINT
#define PRINT 2
        printf("%03d%%\n", PRINT);
        }
        //fflush(stdout);
        i++;
    }
    printf("%d\n", i);
    return 0;
}


