
#include "amessage.pb-c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    AMessage msg;//= AMESSAGE__INIT;
    StructTest  str_test = STRUCT_TEST__INIT; 
    StructTest__Nested2 nest[2];
    AMessage *recv_msg = NULL;
    uint8_t *buf;
    unsigned len;
    int i;
    struct_test__init(&str_test);
    struct_test__nested2__init(&nest[0]);
    struct_test__nested2__init(&nest[1]);
    /*
    struct_test__nested2__init(nest + 1);
    */
    amessage__init(&msg);

    //str_test.nested2 = nest;
    //str_test.nested2 = (StructTest__Nested2 *)malloc(sizeof(StructTest__Nested2));
    nest[0].a = 3;
    nest[0].b = 4;
    nest[1].a = 3;
    nest[1].b = 4;
    str_test.n_nested2 = 2;
    str_test.nested2 = &nest;
    //str_test.nested2[1] = nest[1];
    fprintf(stdout, "1error unpacking message\n");
    len = struct_test__get_packed_size(&str_test);
    fprintf(stdout, "2error unpacking message\n");
    return 0;
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr,"usage: pack a [b]\n");
        return 1;
    }
    msg.a = atoi (argv[1]);
    if (argc == 3) 
    { 
        msg.has_b = 1; 
        msg.b = atoi(argv[2]); 
    }
    len = amessage__get_packed_size (&msg);
    buf = malloc (len);
    amessage__pack (&msg, buf);
    
    for (i = 0; i < len; i++)
    {
        if (i != 0 && i % 16 == 0)
            printf("\n");
        printf("%02x ", buf[i]);
    }
    printf("\n");

    recv_msg = amessage__unpack (NULL, len, buf);
    if (recv_msg == NULL)
    {
        fprintf(stdout, "error unpacking message\n");
        return 1;
    }

    printf ("a=%d", recv_msg->a);
    if (recv_msg->has_b)
        printf ("; b=%d", recv_msg->b);
    printf ("\n");
    return 0;
} 
