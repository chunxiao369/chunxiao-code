
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int status;
    int log_perror;

    /* Make sure we have stdin, stdout and stderr. */
    status = open ("/dev/null", O_RDWR);
    printf("status: %d.\n", status);
    if (status == 0) {
        status = open ("/dev/null", O_RDWR);
        printf("status: %d.\n", status);
    }
    if (status == 1) {
        status = open ("/dev/null", O_RDWR);
        printf("status: %d.\n", status);
        log_perror = 0; /* No sense logging to /dev/null. */
    } else if (status != -1)
        close (status);

    return 0;
}
