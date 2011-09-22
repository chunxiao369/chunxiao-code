#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<dirent.h>

/*
 * The following prints errno=25 (ENOTTY) under 4.3BSD and SVR2,
 * when stdout is redirected to a file.
 * Under SVR4 and 44BSD it works OK.
 */

int main(int argc, char *argv[])
{
    DIR *dp;
    struct dirent *dirp;

	if (argc != 2) {
		printf("usage: ls directory_name.\n ");
		exit(1);
	}
    if ((dp = opendir(argv[1])) == NULL) {
        printf("cant't open %s.\n", argv[1]);
    }

    while ((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);
    closedir(dp);

	exit(0);
}
