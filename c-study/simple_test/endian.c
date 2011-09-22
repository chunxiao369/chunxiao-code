#include <stdio.h>
#include <stdint.h>


int main()
{
    union _u {
        uint16_t a;
        uint8_t  b;
    } u;
    u.a = 0x1234;
    if (u.b == 0x12) {
        printf("big endian.\n");
    } else {
        printf("little endian.\n");
    }
    return 0;
}


