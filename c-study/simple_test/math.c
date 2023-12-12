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

   x = 3.14159;
   y = sin(x/2);
   printf( "sin( %f/2 ) = %f\n", x, y );
   x = asin(y);
   printf( "asin( %f ) = %f\n", y, x );
   /*
   acos(3), asin(3), atan(3), atan2(3), cos(3), csin(3), sincos(3), tan(3)
   */
   return 0;
}
