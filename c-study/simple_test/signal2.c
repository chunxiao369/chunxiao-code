/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigusr1_count = 0;

void handler (int signal_number)
{
  ++sigusr1_count;
  printf ("SIGUSR1 was raised %d times\n", sigusr1_count);
}

int test_const(const int* p)
{
    p++;
    printf("p address %p.\n", p);
}

int main ()
{
  struct sigaction sa;

  int a = 0;
  const int *p = &a;

  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &handler;
  sigaction (SIGKILL, &sa, NULL);
  //sigaction (SIGUSR1, &sa, NULL);
  sigaction (SIGINT, &sa, NULL);
  
  
    printf("p address %p.\n", p);
    test_const(p); 
    printf("p address %p.\n", p);
  /* Do some lengthy stuff here.  */
  /* ...  */
  //while(1);
  printf ("SIGUSR1 was raised %d times\n", sigusr1_count);
  return 0;
}
