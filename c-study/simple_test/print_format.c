#include "stdio.h"

#define  pa_tap_print( format, args...)           printf( "  "format,##args)
#define  ppe_print(s, format, args...)      { \
   printf("  %-30s",s);                              \
   printf(format, ##args);                          \
}
int main()
{
   //int num = 0;
   //void *p;
   //printf("Hello world!%d\n",sizeof(p));
   int i = 5;
   ppe_print("[ chunxiao ]","%d\n",i);
   return 0;
}


#include "stdio.h"

#define  pa_tap_print( format, args...)           printf( "  "format,##args)
#define  ppe_print(s, format, args...)      { \
   printf("  %-30s",s);                              \
   printf(format, ##args);                          \
}
int main()
{
    //int num = 0;
    //void *p;
    //printf("Hello world!%d\n",sizeof(p));
    int i = 5;
    int aaa = 0xabcd;
    ppe_print("[ chunxiao ]","%d\n",i);
   
    fprintf(stderr, "%s\n", "xu chunxiao");
    fprintf(stderr, "%s\n", "who graduated from soochow university, has been working in embedway.");
    fprintf(stdout, "%s\n", "xu chunxiao, who graduated from soochow university, has been working in embedway.");
    
    printf("aaa is 0x%X\n", aaa);
    printf("aaa is 0x%x\n", aaa);


    return 0;
}


