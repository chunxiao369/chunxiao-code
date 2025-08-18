use lscpu to check you cpu support AVX-512
To fix your code and avoid errors, ensure your CPU and compiler support AVX-512 instructions and include the correct compiler flags for AVX-512.

gcc -O2 -mavx512f -o simd simd.c

