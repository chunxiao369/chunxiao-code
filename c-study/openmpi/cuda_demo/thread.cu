#include <cstdio>
#include <cuda_runtime.h>

__global__ void kernel() {
    /*
    printf("same content: gripDim (%d,%d,%d), blockDIM (%d,%d,%d)\n",
           gridDim.x, gridDim.y, gridDim.z,
           blockDim.x, blockDim.y, blockDim.z);
    */
    printf("Block (%d,%d,%d) , Thread (%d,%d,%d) \n",
           blockIdx.x, blockIdx.y, blockIdx.z,
           threadIdx.x, threadIdx.y, threadIdx.z);
}

int main() {

    /* first para is block number, 2 blocks
     * dim(2,1,1) is three-dimensional number from (0,0,0) -> 1,0,0
     */
    /* second para is thread number per block, 16 threads
     * dim(2,2,4) is three-dimensional number from (0,0,0) -> 1,1,3
     */
    kernel<<<dim3(2, 1, 1), dim3(2, 2, 4)>>>();
    cudaDeviceSynchronize();
    return 0;
}

