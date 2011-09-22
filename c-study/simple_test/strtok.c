#include <string.h> 
#include <stdio.h> 
int main(void) 
{
   char input[16] = "abc"; 
   char *p;

   char *a = "def";
   char *b = "abcdefg";
   char *c = "abcdfg";

   p = "ccc";
   printf("aaaaaaaaaaaa%s\n", p);
   printf("---------%s,\n", strstr(b, a));
   printf("---------%s,\n", strstr(c, a));
    /* strtok places a NULL terminator 
   in front of the token, if found */ 
   p = strtok(input, ","); 
   if (p)
   {
      printf("This is the first time!\n");
      printf("%s\n", p); 
   }
   else 
   {
      printf("return null\n");
      return 0;
   }
   /* A second call to strtok using a NULL 
   as the first parameter returns a pointer 
   to the character following the token */ 
   p = strtok(NULL, ","); 
   if (p) 
   {
      printf("This is the second time!\n");
      printf("%s\n", p); 
   }
   p=strtok(NULL,",");
   if(p)
   {
      printf("This is the Third time!");
      printf("%s\n",p);
   }
   
   return 0; 
} 
