#include <cstdio>
#include <cuda_runtime.h>

#define CUDACHECK(cmd)                                              \
  do {                                                              \
    cudaError_t err = cmd;                                          \
    if (err != cudaSuccess) {                                       \
      printf("Failed: Cuda error %s:%d '%s'\n", __FILE__, __LINE__, \
             cudaGetErrorString(err));                              \
      exit(EXIT_FAILURE);                                           \
    }                                                               \
  } while (0)

__global__ void kernel(int *pret) {
    *pret = 42;
}

int main() {
    int *pret;
    CUDACHECK(cudaMalloc(&pret, sizeof(int)));
    kernel<<<1, 1>>>(pret);

    int ret;
    CUDACHECK(cudaMemcpy(&ret, pret, sizeof(int), cudaMemcpyDeviceToHost));
    printf("result: %d\n", ret);

    cudaFree(pret);
    return 0;
}