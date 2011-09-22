/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

/*
 * parent process and child process
 * share the code segment
 * own
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int test = 1;
pthread_t config_id;
void *test_a(void *param)
{
  while (1) {
  printf(" pid is %d.\n", getpid());
  sleep(1);
  }
}

static void config_success_signal(int sig)
{
    switch (sig) {
        case SIGUSR1:
            printf("....find child send signal.\n");
            break;
        case SIGCHLD:
            printf("....find child exit.\n");
            wait(0);
            break;
        default:
            break;
    }
}

int main ()
{
  pid_t child_pid;
  //int i;

  printf ("the main program process id is %d\n", (int) getpid ());
  //pthread_create(&config_id, NULL, test_a, NULL);

  child_pid = fork();
  signal(SIGUSR1, config_success_signal);
  signal(SIGCHLD, config_success_signal);
  if (child_pid != 0) {
    //printf ("this is the parent process, with id %d\n", (int) getpid ());
    //printf ("the child's process id is %d\n", (int) child_pid);
    test = 3;
    //printf("test is parent %d, %p.\n", test, &test);
    while (1) {
        sleep(2);
    }
  } else {
    char tmp[20] = {0};
    strncpy(tmp, "aaaaaaaaaaaaaaaaaaaaaaaaaaa", 21);
    kill(getppid(), SIGUSR1);
    printf ("    this is the child process, with id %d\n", (int) getpid ());
    /*
    for (i = 0; i < 10; i++)
      printf("test is child %d, %p.\n", test, &test);
    */
    exit(0);
  }
  return 0;
}
