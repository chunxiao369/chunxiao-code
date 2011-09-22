
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

char* stripwhite( char *string )
{
    register char *s, *t;
    
    for ( s = string; whitespace( *s ); s++ ) {
    }
    
    if ( *s == 0 ) {
        return s;
    }
    
    t = s + strlen( s ) - 1;
    while ( t > s && whitespace( *t ) ) {
        t--;
    }
    *++t = '\0';

    return s;
}
int main(int argc, char *argv[])
{
    char prompt[ 16 ];
    char *line;
    char *s = NULL;
    struct termios term;

    memset( prompt, 0, 16 );
    sprintf( prompt, "CLI>" );
    //printf("cli>");
    tcgetattr(fileno(stdin), &term);
    tcsetattr(fileno(stdin), TCSAFLUSH, &term);
    fflush(stdin);
    /* receive command line input. */
    //hy_switch_to_buffer(stdin);

    while ( 1 ) {
        line = readline( prompt );
        if ( !line ) {
            break;
        }
        s = stripwhite( line );
        if (strcmp(s, "exit") == 0)
            break;
        if ( *s ) {
            add_history ( s );
            printf("Input is %s.\n", s);
        }
        //printf("cli>");
        free( line );
    }
    return 0;
}

