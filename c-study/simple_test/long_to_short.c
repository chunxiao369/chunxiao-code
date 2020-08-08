#include "stdio.h"
#include "string.h"
int get_short_string_from_long(char *long_string, char short_string[24][15])
{
	int num_short_string=0;
	char temp_long_string[250]={0},*p;
	strcpy(temp_long_string,long_string);
	p = strtok(temp_long_string, ",");

	while(p)
	{
		strcpy(short_string[num_short_string],p);
		p = strtok(NULL, ",");
		num_short_string++;
	}
	return num_short_string;
}
int main()
{
	char buf[250]={0};
	int len,i;
	char temp_short_ge[24][15];
	strcpy(buf,"1/f/23,1/f/24,,,2/f/3");
	len=get_short_string_from_long(buf,temp_short_ge);
	printf("%s\n",buf);
	for(i=0;i<len;i++)
	printf("%s\n",temp_short_ge+i);
}

