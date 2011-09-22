#include <stdio.h>
//#include "../octpow/com_octpow_sendpacket.h"

typedef struct{
	char str[1];
	int array[0];
} node; 


int main(int argc, char **argv)
{
	//octpow_Initialize();
    //octpow_one_msg_start(); 
    //octpow_Finalize();
    node a;
    printf("The node is %d\n",sizeof(a));
	return 0;
}
