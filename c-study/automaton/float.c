
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char *nexttoken();
char nextchar();
void next();
void back();
char* gettoken();

char cbuf[]="12.3*********klj12.2e2jj778";
int forward = -1;



int main()
{
    while(1) 
    {
        printf("%s\n", nexttoken());
        if(forward >= strlen(cbuf) - 1)
        {
            //getchar();
            return 0;
        }
    }
}

int state;
int start;

char* nexttoken()
{
    char c;
    state = 12;
    while (1)
    {
        switch (state)
        {
        case 12:
            c = nextchar();
            start = forward;
            if(isdigit(c))
            {
                state = 13;
            }
            else
            {
                next();
            }
            break;
        case 13:
            c = nextchar();
            if (isdigit(c))
                state = 13;
            else if (c == 'e'||c == 'E')  
                state = 16;
            else if (c == '.')
                state = 14;
            else
                state = 19;
            break;
        case 14:
            c = nextchar();
            if (isdigit(c))
                state = 15;
            break;
        case 15:
            c = nextchar();
            if (isdigit(c))
                state = 15;
            else if (c == 'e'|| c == 'E')
                state = 16;
            else
                state = 19;
            break;
        case 16:
            c = nextchar();
            if (isdigit(c))
                state = 18;
            else if (c == '+' || c == '-')
                state = 17;
            break;
        case 17:
            c = nextchar();
            if (isdigit(c))
                state = 18;
            break;
        case 18:
            c = nextchar();
            if (isdigit(c))
                state = 18;
            else
                state = 19;
            break;
        case 19:
            back();
            return gettoken();
        }
    }
}

char nextchar()
{
    forward++;
    return cbuf[forward];
}

void back()
{
    forward--;
}

void next()
{
    forward++;
}

char token_buf[128];
char* gettoken()
{
    int i,j=0;
    for(i = start; i <= forward; i ++)
    {
        token_buf[j++] = cbuf[i];
    }
    token_buf[j] = '\0';
    return token_buf;
}


