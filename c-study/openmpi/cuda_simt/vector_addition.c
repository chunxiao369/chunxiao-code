#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "cuda_runtime.h"
#include "kernel.h"

// declare the vectors' number of elements and their size in bytes
static const int n_el = 8;
static const size_t size = n_el * sizeof(float);

int main(){
  // declare and allocate input vectors h_A and h_B in the host (CPU) memory
  float* h_A = (float*)malloc(size);
  float* h_B = (float*)malloc(size);
  float* h_C = (float*)malloc(size);

  // declare device vectors in the device (GPU) memory
  float *d_A,*d_B,*d_C;

  // initialize input vectors
  for (int i=0; i<n_el; i++){
    h_A[i]=sin(i);
    h_B[i]=cos(i);
  }

  // allocate device vectors in the device (GPU) memory
  cudaMalloc((void **)&d_A, size);
  cudaMalloc((void **)&d_B, size);
  cudaMalloc((void **)&d_C, size);

  // copy input vectors from the host (CPU) memory to the device (GPU) memory
  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

  // call kernel function
  asum(d_A, d_B, d_C, n_el);

  // copy the output (results) vector from the device (GPU) memory to the host (CPU) memory
  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
  // free device memory
  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  // compute the cumulative error
  double err=0;
  for (int i=0; i<n_el; i++) {
    double diff = (double)((h_A[i]+h_B[i])-h_C[i]);
    err+=diff*diff;
    // print results for manual checking.
    printf("A+B: %f, ", h_A[i]+h_B[i]);
    printf("C: %f\n", h_C[i]);
  }
  err=sqrt(err);
  printf("err: %f\n", err);

  // free host memory
  free(h_A);
  free(h_B);
  free(h_C);

  return cudaDeviceSynchronize();
}
