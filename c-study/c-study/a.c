#include <stdio.h>
#include <string.h>
static void l7_pattern_expand(char *string)
{
	char buf[512]={0};
	int i=0,j=0;
	while(*(string+j)){
		switch(*(string+j)){//^ $[]{}()<>\*.?+|
		case '\\':
			if(*(string+j+1)=='"'){
				j++;
				continue;
			}
		case '^':
		case ' ':
		case '$':
		case '[':
		case ']':
		case '{':
		case '}':
		case '(':
		case ')':
		case '<':
		case '>':		
		case '*':
		case '.':
		case '\?':
		case '+':
		case '|':
			buf[i++]='\\';
			break;
		}
		buf[i]=*(string+j);
		i++;
		j++;
	}	
	strcpy(string,buf);

}

int main()
{
    char str[100] = "^aaa";
    l7_pattern_expand(str);
    printf("str: %s.\n", str);
    return 0;
}
