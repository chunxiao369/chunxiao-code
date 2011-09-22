/*   
  *   gcc   -Wall   -pipe   -g   -static   -o   myprog_2   myprog_2.c   
*/   
  
#include   <stdio.h>   
#include   <stdlib.h>   
#include   <unistd.h>   
  
#define   MAXBUFSIZE   1024   
 
int  main(int   argc, char *argv[])   
{   
      char buf[   MAXBUFSIZE   ];   
      int  count;   
      count = readlink(   "/proc/self/exe",   buf,   MAXBUFSIZE   );   
  
      if(count < 0 || count >= MAXBUFSIZE)   
      {   
              printf(   "Failed\n"   );   
              return(   EXIT_FAILURE   );   
      }   
      buf[   count   ]   =   '\0';   
      printf(   "/proc/self/exe   ->   [%s]\n",   buf   );   
      return(   EXIT_SUCCESS   );   
}     /*   end   of   main   */   
  
