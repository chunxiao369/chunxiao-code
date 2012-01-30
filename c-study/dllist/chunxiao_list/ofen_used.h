
#ifndef __OFEN_USED_H__
#define __OFEN_USED_H__

#define return_if_fail(p) if(!(p)) \
    {printf("%s:%d Warning: "#p" failed.\n", \
        __func__, __LINE__); return;}

#define return_val_if_fail(p, ret) if(!(p)) \
    {printf("%s:%d Warning: "#p" failed.\n",\
    __func__, __LINE__); return (ret);}

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

static void print_data(unsigned char *ptr, int len)
{
    int i;
    printf("ptr start at %p, len: %d.\n", ptr, len);
    for (i = 0; i < PACKET_SIZE; i++)
    {
        if ((i % 16 == 0) && (i != 0))
            printf("\n");
        printf("%02x ", ptr[i]);
    }
    printf("\n");
}

#endif
