#include <stdio.h>

int execute_line( char *line )
{
	register char *s, *t;
	int state;	
	int i;
	process_cmd_fn_t cmd_fn;
	int match_arg;
	int cmd_argc;
	char *cmd_argv[ 16 ];

	cmd_argc = 0;
	s = line;
	t = line;
	state = 0;
	while ( *t ) {
		if ( ( state == 0 ) && ( whitespace( *t ) ) ) {
			cmd_argv[ cmd_argc ] = s;
			cmd_argc++;
			*t = 0;
			state = 1;
			if ( cmd_argc >= 16 ) {
				goto parse_ok;
			}
		} else if ( ( state == 1 ) && ( !whitespace( *t ) ) ) {
			s = t;
			state = 0;
		}
		t++;
	}
	cmd_argv[ cmd_argc ] = s;
	cmd_argc++;

parse_ok:
	if ( cmd_argc == 1 ) {
		cmd_fn = find_cmd_fn( cmd_argv[ 0 ], NULL, cmd_list, CMD_CNT, &match_arg );
	} else {
		cmd_fn = find_cmd_fn( cmd_argv[ 0 ], cmd_argv[ 1 ], cmd_list, CMD_CNT, &match_arg );
	}
	if ( cmd_fn ) {
		return cmd_fn( cmd_argc - match_arg, cmd_argv + match_arg );
	} else {
		printf( "unknown command\n" );
	}
	
	return 0;
}

int main()
{
	char *line = NULL;
	char *s = NULL;
	while ( 1 ) {
		line = readline( prompt );
		if ( !line ) {
			break;
		}
		s = stripwhite( line );
		if ( *s ) {
			add_history ( s );
			execute_line( s );
		}
	}
	free( line );
}
