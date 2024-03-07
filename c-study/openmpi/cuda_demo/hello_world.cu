#include <cstdio>
#include <cuda_runtime.h>
/*
#include <helper_cuda.h>
int CUDACores = _ConvertSMVer2Cores(prop.major, prop.minor) * prop.multiProcessorCount;
*/
__device__ void say_hello() {
    printf("Hello, world from GPU!\n");
}

__host__ void say_hello_host() {
    printf("Hello, world from CPU!\n");
}

__global__ void kernel() {
    say_hello();
}

int main() {
    int device_count;
    /* first para is block number*/
    /* second para is thread number per block*/
    kernel<<<1, 2>>>();
    cudaDeviceSynchronize();
    say_hello_host();
    cudaGetDeviceCount(&device_count);
    for (int i = 0; i < device_count; ++i) {
        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, i);
        printf("Device %d: %s, has ", i, prop.name);
        printf("SM Count: %d\n", prop.multiProcessorCount);
    }

    return 0;
}

