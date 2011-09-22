#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *ptr;
    ptr = (char *)0x8000000017060001;
    if (argc != 2) {
        printf("error param.\n");
        return 1;
    }
    printf("%s\n", argv[1]);
    if (strncmp(argv[1], "0x70", 4) == 0) {
        *ptr = 0x70; 
        printf("70.\n");
    } else if (strncmp(argv[1], "0x10", 4) == 0) {
        *ptr = 0x10; 
        printf("10.\n");
    }

    return 0;
}

