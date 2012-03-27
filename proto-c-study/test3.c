
#include "amessage.pb-c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buf[] = {0x65 ,0x6d ,0x62 ,0x65 ,0x64 ,0x77 ,0x61 ,0x79 ,0x0a ,0x00 ,0x10 ,0x00 ,0x18 ,0xe5 ,0x02 ,0x20 ,0x18 ,0x28 ,0x02 ,0x52 ,0x02 ,0x08 ,0x00 ,0x65 ,0x6d ,0x62 ,0x65 ,0x64 ,0x77 ,0x61 ,0x79};
int main(int argc, char **argv)
{
    CommPkt *recv_msg = NULL;
    printf("len: %d.\n", sizeof(buf));
    recv_msg = comm_pkt__unpack(NULL, 31 - 16, buf + 8);
    if (recv_msg == NULL)
    {
        fprintf(stdout, "error unpacking message\n");
        return 1;
    }

    printf ("seqno%d.\n", recv_msg->seqno);
    printf ("opcode%d.\n", recv_msg->opcode);
    printf ("\n");
    return 0;
} 
