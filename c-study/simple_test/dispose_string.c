#include <stdio.h>
#include <string.h>

typedef unsigned int uint8_t;
typedef unsigned long uint64_t;

uint64_t get_integer( char *str, char **end_ptr )
{
	if ( strncmp( str, "0x", 2 ) == 0 ) {
		return strtoll( str, end_ptr, 16 );
	} else {
		return strtoll( str, end_ptr, 10 );
	}
}
int get_token_count( char *buffer, char delimit, int *count )
{
	char *ptr;
	char *ptr2;
	
	*count = 0;
	ptr = buffer;
	ptr2 = ptr;
	while ( *ptr ) {
		if ( *ptr == delimit ) {
			if ( ptr == ptr2 ) {
				return -1;
			}
			*count = *count + 1;
			ptr++;
			ptr2 = ptr;
		} else {
			ptr++;
		}
	}

	if ( ptr == ptr2 ) {
		return -1;
	}
	*count = *count + 1;
	return 0;
}

void get_token_list( char *buffer, char delimit, char **token_list )
{
	char *ptr;
	char *ptr2;
	int count;

	count = 0;
	ptr = buffer;
	ptr2 = ptr;
	while ( *ptr ) {
		if ( *ptr == delimit ) {
			*ptr = '\0';
			token_list[ count++ ] = ptr2;
			ptr++;
			ptr2 = ptr;
		} else {
			ptr++;
		}
	}

	token_list[ count ] = ptr2;
}

int get_range( char *str, uint64_t *low, uint64_t *high )
{
	char *ptr;
	uint64_t l, h;
	char *end_ptr;

	ptr = strchr( str, '-' );
	if ( ptr ) {
		*ptr = '\0';
		l = get_integer( str, &end_ptr );
		if ( *end_ptr != 0 ) {
			return -1;
		}
		h = get_integer( ptr + 1, &end_ptr );
		if ( *end_ptr != 0 ) {
			return -1;
		}
		if ( h < l ) {
			l = l ^ h;
			h = l ^ h;
			l = l ^ h;
		}
	} else {
		l = get_integer( str, &end_ptr );
		if ( *end_ptr != 0 ) {
			return -1;
		}
		h = l;
	}
	
	*low = l;
	*high = h;
	return 0;
}
int get_int_list( char *str, uint8_t *int_list, uint64_t *int_count )
{
	int result = 0;
	int token_count;
	char **token_list;
	int i, j;
	int int_id;	
	uint64_t high, low;
	int count = 0;
	
	result = get_token_count( str, ',', &token_count );
	if ( ( result < 0 ) || ( token_count > 256 ) ) {
		return -1;
	}
	token_list = ( char ** )malloc( sizeof( char * ) * token_count );
	if ( token_list == NULL ) {
		return -2;
	}
	get_token_list( str, ',', token_list );

	for ( i = 0; i < token_count; i++ ) {
		result = get_range( token_list[ i ], &low, &high );
		if ( result < 0 ) {
			free( token_list );
			return -1;
		}
		if ( ( low < 1 ) || ( low > 32 ) ||
			( high < 1 ) || ( high > 32 ) ) {
			free( token_list );
			return -1;
		}
		for ( j = low; j <= high; j++ ) {
			if ( count > 255 ) {
				free( token_list );
				return -1;
			}
			int_list[ count ] = j - 1;
			count++;
		}
	}
	*int_count = count;
	free( token_list );
	
	return result;
}

int main(int argc, char *argv[])
{
	uint8_t port_list[ 256 ] = {0};
	uint64_t int_count = 0;
	int i = 0;
	get_int_list( argv[ 1 ],port_list,&int_count );
	for (i = 0; i < int_count; i++)
	{
		printf("%d  ", port_list[ i ]);
	}
	printf("\nThe port num is: %d\n", int_count);
}
