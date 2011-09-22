/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define FILE_LENGTH 0x100

int main (int argc, char* const argv[])
{
  int fd;
  void* file_memory;
  int integer;
  printf("%s...\n", __FILE__);
  /* Open the file.  */
  fd = open (argv[1], O_RDWR, S_IRUSR | S_IWUSR);
  printf("%s...\n", __FILE__);
  /* Create the memory-mapping.  */
  file_memory = mmap (0, FILE_LENGTH, PROT_READ | PROT_WRITE,
		      MAP_SHARED, fd, 0);
  close (fd);
  printf("%s, %d....\n", __FILE__, __LINE__);
  
  printf("file_memory is %p.\n", file_memory);
  /* Read the integer, print it out, and double it.  */
  sscanf (file_memory, "%d", &integer);
  printf ("value: %d\n", integer);
  sprintf ((char*) file_memory, "%d\n", 2 * integer);
  /* Release the memory (unnecessary since the program exits).  */
  munmap (file_memory, FILE_LENGTH);

  return 0;
}
