#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

int main()
{	
    FILE *fp;
    DIR *dp;
    char file_name[256];
    int return_value;
    
    sprintf(file_name,"xu-test");
    return_value = access( file_name,F_OK );
    if(return_value == -1)
    {
        printf("file not access!\n");
        return -99;
    }
    fp = fopen( file_name, "rb" );
    if ( fp == NULL )
    {
        printf( "[ FAILED ] open config file error\n" );
        return -100;
    }
    else 
    {
        printf("[ OK ] open config file right!\n");
    }
    dp = opendir("abc");
    if(dp == NULL)
        printf("abc not existed!\n");
    
    return 0;
}

