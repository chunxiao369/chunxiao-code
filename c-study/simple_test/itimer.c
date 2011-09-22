/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

static int count = 0;
void timer_handler (int signum)
{
    printf ("timer expired %d times\n", ++count);
    alarm(2);
}

int main ()
{
    signal(SIGALRM, timer_handler);
    alarm(1);
    while(1) {
        printf("aaaaa.\n");
        sleep(1);
        if (count > 10)
            alarm(0);
    }
    return 0;
}

