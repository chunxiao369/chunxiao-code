#include <stdio.h>
#include <stdint.h>

int main()
{
    char aa = 0x29;
    uint16_t bb = 0xfffa;
    uint16_t cc = 0;
    if (cc > bb) {
        printf("aaaaaaaaaaaaaaaaaaaa, %d.\n", (cc -bb));
    }
    printf("aaaaaaaaaaaaaaaaaaaa, %d.\n", (cc -bb));

    printf("%x\n",aa );
    printf("%x\n",(uint16_t)aa);
    bb = (uint8_t)aa;
#ifdef CHUN
    printf("CHUN\n");
#endif 

#ifdef XIAO 
    printf("XIAO\n");
#endif 
    if( bb > 255)
        printf("aa is bigger. \n");
    
    return 0;
}
