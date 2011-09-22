#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fin;
	unsigned char str[1024];
	unsigned char aa = 0;
	int i = 0;
    char *tmp;
    int num = 0;
    

	fin = fopen( argv[1], "r" );
	if(fin == NULL)
	{
			printf("Open the in-file error!\n");
			return 0;
	}
    fgets(str,1024,fin);
    printf("str=%s\n",str);
	while(!feof(fin))
	{
        i++;
        fgets(str, 1024, fin);
        str[strlen(str)-1] = 0;
        printf("%s************\n",str);
        {///////////////////////////////////
           tmp = strtok( str, " ");
           printf("\t%s  --->   ",tmp);
           printf("%ld\n",inet_ntoa(tmp));
           num = 0;
           do
           {
           num++;
           tmp = strtok( NULL, " ");
           printf("\t%s-------------------\n",tmp);
           }
           while( tmp != NULL);
        }////////////////////////////////////
	}

	printf("num is %d.\n", i);
	fclose(fin);
    return 0;
}
