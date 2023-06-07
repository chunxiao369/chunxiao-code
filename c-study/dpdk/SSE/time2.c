#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <nmmintrin.h>
#include <immintrin.h>

//#include <stdio.h>
//#include <x86intrin.h>

__m512i vectorFibo(__m512i a, __m512i b, size_t number)
{
  __m512i t =_mm512_set1_epi32(0);
  while((number --) != 0) {
    t += _mm512_add_epi32(a,b);
  }
  return t;
}


int main()
{
  __m512i a =_mm512_set1_epi32(0);
  __m512i b =_mm512_set1_epi32(1);
  __m512i c = vectorFibo(a,b,1000000);
  //__m512i c = vectorFibo(a,b,1);
  uint32_t *p = (uint32_t *)&c;
  for (int i = 0; i < 16; i++) {
	  printf("p[%d] == %u\n", i, p[i]);
  }
  int returnval =  _mm256_extract_epi32( _mm512_extracti64x4_epi64(c,1),7);
  printf("extract %d \n", returnval);
  return returnval;
}

