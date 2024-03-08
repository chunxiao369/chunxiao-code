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
    *pret = 666;
}

__global__ void just_print(int *gp) {
    printf("from GPU print vlaue: %d\n", *gp);
}


int main() {
    int *pret;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    CUDACHECK(cudaMalloc(&pret, sizeof(int)));
    kernel<<<1, 1>>>(pret);

    int ret;
    CUDACHECK(cudaMemcpy(&ret, pret, sizeof(int), cudaMemcpyDeviceToHost));
    printf("from CPU print value: %d\n", ret);

    cudaEventRecord(start);
    /* GPU calc process*/
    cudaEventRecord(stop);

    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    ret += 222;
    CUDACHECK(cudaMemcpy(pret, &ret, sizeof(int), cudaMemcpyHostToDevice));
    just_print<<<1, 1>>>(pret);
    //printf("result: %d\n", *pret);
    cudaFree(pret);
    return 0;
}
