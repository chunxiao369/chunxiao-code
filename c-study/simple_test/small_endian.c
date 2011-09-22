#include <stdio.h>
unsigned char chk_cpu()
{
  int data = 0x0011;
  char temp = *(char *)&data;
  if(temp == 0)
  {
     printf("big endian!\n");
     return 0; //大端返回0
  }
  else 
  { 
    printf("small endian!\n");
    return 1;   //小段返回1
  }
}
union endian_test
{
  short short16;
  struct 
  {
    int first   : 7;
    int second  : 1;
    int third   : 2;
    int four    : 6;
  }s;
};
int main()
{
  chk_cpu();
  union endian_test vlue01;
  vlue01.short16 = 273;
  printf("first:%x,second:%x,third:%x,four:%x\n",vlue01.s.first,\
                                               vlue01.s.second,\
                                               vlue01.s.third,\
                                               vlue01.s.four);
}

