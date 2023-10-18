#include <math.h>
#include <stdio.h>

int main( void )
{
   double x = 1;
   //double x = 2.302585093;
   double y;
   double z;

   y = exp(x);
   z = log(y);
   printf( "exp( %f ) = %f\n", x, y );
   printf( "log( %f ) = %f\n", y, z );
   x = 2;
   y = 3;
   z = pow(2, 3);
   printf( "pow( %f, %f ) = %f\n", x, y, z );
   return 0;
}
