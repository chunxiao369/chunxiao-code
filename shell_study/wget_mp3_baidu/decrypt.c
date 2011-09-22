#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int K[200], H[200];

void N(int S, int P, int Q)
{
	int R;
	for(R=S;R<=P;R++)
	{
		K[R]=R+Q;
		H[R+Q]=R;
	}
}


int main(int argc, char *argv[])
{
	//char *s = "2EEA://7AA-Q.w997AO.x97/z825ynTJhYVPgKVQgKdLgK6Ci8iWwKeRdNhIjrUHiKeAxMTN.7AO";
	//int dis = (unsigned long)1223268873%26;
		
    char *s = argv[1];
    int dis = ((unsigned long)atoi(argv[2]))%26;
    int i, value;

	dis = dis?dis:1;
	N(0,9,48);
	N(10,35,55);
	N(36,61,61);

	for(i=0; i<strlen(s); i++){
		if((s[i]>='a' && s[i]<='z') || (s[i]>='0' && s[i]<='9') || (s[i]>='A' && s[i] <='Z')){
			value =  H[(int)s[i]]-dis;
			if(value<0) value+=62;
			value = K[value];
		}
		else
			value = s[i];
		
		printf("%c", value);
	}
	printf("\n");
	return 0;
}
