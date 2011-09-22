
/**
 * ./xuexe -a 
 * ./xuexe -a 111
 * ./xuexe -b 
 * ./xuexe -b 111
 * ./xuexe -c 
 * You'll understand how to use getopt after seeing the result.
 *
 */
#include<stdio.h>
#include<unistd.h>
int main(int argc,char **argv)
{
    int ch;
    //opterr = 0;
    while((ch = getopt(argc, argv, "a:bcde"))!= -1)
    {
        switch(ch)
        {
        case 'a':
            printf("option a optarg is :'%s'\n",optarg);
            break;
        case 'b':
            printf("option b\n");
            break;
        default:
            printf("other option :%c\n",ch);
        }
    }
    printf("ssssssssssss +%c\n",optopt);
    return 0;
}

