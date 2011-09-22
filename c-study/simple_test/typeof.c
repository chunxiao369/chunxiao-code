#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
typedef struct 
{
     int x;
      char y;
}astruct, * pastrcut;

int main()
{
     int sizem, sizew;
     int x = 3;
     printf("aaaaaaaaaaaa. %d.\n", offsetof(astruct, x));
     typeof(&x) m;
     printf("aaaaaaaaaaaa. %d.\n", offsetof(astruct, y));
     sizem = sizeof(m);
     printf("aaaaaaaaaaaa.\n");
     m = &x;
     *m = 5;
     printf("aaaaaaaaaaaa.\n");
     typeof(((astruct *)5)->y) w;
     sizew = sizeof(w);
     w = 'a';
     return 1;
}
