#include <stdio.h>
#include <math.h>
#include "cuda_runtime.h"
#include "kernel.h"

#define THREADS_NUM 8
// declare the kernel function
__global__ void kernel_sum(const float* A, const float* B, float* C, int n_el);

// function which invokes the kernel
void asum(const float* A, const float* B, float* C, int n_el) {

  // declare the number of blocks per grid and the number of threads per block
  int threadsPerBlock,blocksPerGrid;

  // use 1 to THREADS_NUM threads per block
  printf("n_el: %d.\n", n_el);
  if (n_el< THREADS_NUM){
    threadsPerBlock = n_el;
    blocksPerGrid   = 1;
  } else {
    threadsPerBlock = THREADS_NUM;
    blocksPerGrid   = ceil(double(n_el)/double(threadsPerBlock));
  }
  printf("blocks: %d, threads: %d\n", blocksPerGrid,threadsPerBlock);
  // invoke the kernel
  kernel_sum<<<blocksPerGrid,threadsPerBlock>>>(A, B, C, n_el);
}

// kernel
__global__ void kernel_sum(const float* A, const float* B, float* C, int n_el)
{
  // calculate the unique thread index
  int tid = blockDim.x * blockIdx.x + threadIdx.x;
  printf("dim: %d, block: %d, thread: %d.\n", blockDim.x, blockIdx.x, threadIdx.x);
  // perform tid-th elements addition 
  if (tid < n_el) C[tid] = A[tid] + B[tid];
}
