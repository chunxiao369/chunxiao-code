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
}

int main ()
{
  struct sigaction sa;
  struct itimerval timer;

  /* Install timer_handler as the signal handler for SIGVTALRM.  */
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &timer_handler;
  sigaction (SIGVTALRM, &sa, NULL);

  /* Configure the timer to expire after 250 msec...  */
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 250000;
  /* ... and every 250 msec after that.  */
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 250000;
  /* Start a virtual timer.  It counts down whenever this process is
     executing.  */
  //setitimer (ITIMER_VIRTUAL, &timer, NULL);
  setitimer (ITIMER_REAL, &timer, NULL);

  /* Do busy work.  */
  while(1) {
      //if (count >= 10) {
          sleep(1);
          //signal(SIGVTALRM, SIG_IGN);
          //return 1;
      //}
  }
  return 0;
}
