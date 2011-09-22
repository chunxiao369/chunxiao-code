#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
char szStr[255] = "                                        ";
const int total_len = 40;
void drawper(int cur, int total, char* spOut) 
{

  static int iCrlmm = 0;
  int iPer = cur * 100 / total;
  /*
  if(cur == total)
    sprintf(spOut, "\n");
  else
    sprintf(spOut, "\r");
    return;
  */
  if(cur == total)
    sprintf(spOut, " [ %03d%% ]\n",  iPer);
  else if( iCrlmm == 0)
    sprintf(spOut, "[| %03d%% ]\r",  iPer);
  else if( iCrlmm == 1)
    sprintf(spOut, "[/ %03d%% ]\r",  iPer);
  else if( iCrlmm == 2)
    sprintf(spOut, "[- %03d%% ]\r",  iPer);
  else if( iCrlmm == 3)
    sprintf(spOut, "[\\ %03d%% ]\r", iPer);
 
  if(iCrlmm==3) 
    iCrlmm=0;
  else
    iCrlmm++;
}

void proessview(int cur, int total)
{
  char cTmp       = '#';
  char szOut[10]  = "";
  int i = 0;
  int j = 0;
  if (cur > total)
      return;
  i = (total_len * cur / total); 
  memset(szOut, 0, sizeof(szOut));
  for (j = 0; j <=i; j++) {
      szStr[j] = cTmp;
  }
  drawper(i, total_len, szOut);
  memcpy(szStr + total_len, szOut, strlen(szOut));
  fseek(stdout, 0, SEEK_SET);
  fprintf(stdout, szStr);
  fflush(stdout);
}

main()
{
   int i = 0;
   for (i = 0; i <= 100; i+=1) {
       usleep(500000);
       proessview(i, 100);
   }
}
