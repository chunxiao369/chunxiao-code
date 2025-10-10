#include <stdio.h>
#include <math.h>
#include "cuda_runtime.h"
#include "kernel.h"


// 检查 CUDA API 调用是否成功的宏
#define CUDA_CHECK(call)                                      \
    do {                                                      \
        cudaError_t err = call;                               \
        if (err != cudaSuccess) {                             \
            fprintf(stderr, "CUDA error at %s:%d code=%d (%s)\n", \
                    __FILE__, __LINE__, err, cudaGetErrorString(err)); \
            exit(EXIT_FAILURE);                               \
        }                                                     \
    } while (0)

// -------------------------------------------------------------
// CUDA Kernel: 使用共享内存的并行求和
// -------------------------------------------------------------
__global__ void reduce_sum_shared(const int* g_input, int* g_output, int N) {
    // 1. 软件层级：Thread/Block 抽象
    // ------------------------------------------------
    // blockDim.x：线程块中的线程数 (Threads per Block)
    // blockIdx.x：当前线程块的索引 (Block Index)
    // threadIdx.x：当前线程在块内的索引 (Thread Index)
    
    // 线程在整个 Grid 中的全局索引
    // 一次kernel调用只有一个grid
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (0 == threadIdx.x && 0 == blockIdx.x) {
        printf("gridx %d, gridy %d gridz %d.\n", gridDim.x, gridDim.y, gridDim.z);
        printf("blocks per grid: %d, threads per block: %d, block index: %d, thread index: %d.\n", gridDim.x, blockDim.x, blockIdx.x, threadIdx.x);
    }
    
    // 线程在块内的局部索引
    int local_tid = threadIdx.x;
    
    // 2. 硬件层级：Shared Memory 和 Bank Conflict 优化
    // ------------------------------------------------
    // __shared__ 声明：数据分配在 SM 上的共享内存中 (非常快速，但容量小)
    // 这种访问模式可以很好地避免 Bank Conflict。
    extern __shared__ int sdata[]; // 动态分配共享内存 (大小在启动时指定)
    //share memory访问延迟约为全局内存（Global Memory）的 1/100，带宽是全局内存的 7~10 倍。

    // 将全局内存数据加载到共享内存
    if (tid < N) {
        sdata[local_tid] = g_input[tid];
    } else {
        // 如果数据量不是 blockDim.x 的整数倍，填充0
        sdata[local_tid] = 0; 
    }
    
    // 同步：确保所有线程都已完成从全局内存到共享内存的加载
    __syncthreads(); 

    // ------------------------------------------------
    // 3. 硬件层级：Warp/SM 的并行归约计算
    // ------------------------------------------------
    // 归约求和：通过并行迭代，将一个 Block 的所有值求和到一个值
    
    // 循环步长 (stride) 从 blockDim.x / 2 开始
    for (int stride = blockDim.x / 2; stride > 0; stride /= 2) {
        
        // __syncthreads() 确保每一轮归约完成后，线程才能继续下一轮
        __syncthreads(); 
        
        // 只有线程索引小于步长的线程参与计算
        if (local_tid < stride) {
            // 将 sdata[local_tid + stride] 的值加到 sdata[local_tid]
            // 这就是并行归约的核心操作
            sdata[local_tid] += sdata[local_tid + stride];
            if (0 == threadIdx.x && 0 == blockIdx.x) {
                printf("sum: %d.\n", sdata[0]);
            }
            
            // 硬件配合理解：
            // 第一轮，sdata的后128个各加到前128个。第二轮，sdata的后64个各加到前64个。第三轮后32各加到前32。
            // 在这一步，归约操作由 SM 上的多个 Warp 并行执行。
            // 由于数据在共享内存中，访问速度快，且 sdata[local_tid] 和 sdata[local_tid + stride]
            // 在内存中往往是分散在不同的 Bank，可以实现无 Bank Conflict 的并行访问。
            // 在 NVIDIA GPU 架构中，共享内存被分割成许多独立的物理存储单元，称为 Banks（存储体）。
            // Bank 数量： 现代 NVIDIA GPU 通常将共享内存分割成 32 个或 64 个 Bank。
            // 地址映射： 共享内存中的连续 4 个字节（一个字/word）通常会被映射到 连续的 Bank 中。
            // sdata[local_tid] 和 sdata[local_tid + 1] （即相邻的 4 字节整数）通常属于不同的 Bank。

        }
    }
    // 4. 结果写回：将 Block 结果写回全局内存
    // ------------------------------------------------
    // 只有 Block 的第一个线程 (local_tid == 0) 将最终结果写回全局输出数组
    if (local_tid == 0) {
        // g_output 的大小等于 Grid 的大小（即 Block 的数量）
        g_output[blockIdx.x] = sdata[0];
    }
}

/*
 * NVIDIA Ampere 架构（包括 A100/A800）的共享内存被分割为 32 个 Bank。
 * 这些 Bank 支持一个 Warp（32 个线程）在不发生冲突的情况下，
 * 对连续 32 个 4 字节（32-bit word）的访问，即 32 个线程可以同时访问 32 个不同的 Bank。
 * L1/Shared Memory： Ampere 架构允许每个 Streaming Multiprocessor (SM) 
 * 将 L1 数据缓存和共享内存配置为不同的组合（例如，最高 192KB 的组合容量），但 Bank 的基本数量仍然保持 32 个。
 */
// -------------------------------------------------------------
// Host 代码 (CPU)
// -------------------------------------------------------------
int main() {
    // 数组总大小
    //const int N = 1024 * 1024; 
    const int N = 1024 * 8; 
    // 每个线程块的线程数 (通常是 2 的幂，推荐 256/512/1024)
    const int THREADS_PER_BLOCK = 256; 
    // 计算需要的线程块数
    const int NUM_BLOCKS = (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK; 
    
    // 申请主机内存
    int* h_input = (int*)malloc(N * sizeof(int));
    
    // 初始化输入数据
    for (int i = 0; i < N; ++i) {
        h_input[i] = 1; // 方便验证，最终结果应为 N
    }

    // 申请设备内存
    int *d_input, *d_output;
    CUDA_CHECK(cudaMalloc((void**)&d_input, N * sizeof(int)));
    CUDA_CHECK(cudaMalloc((void**)&d_output, NUM_BLOCKS * sizeof(int)));

    // 将输入数据从主机传输到设备
    CUDA_CHECK(cudaMemcpy(d_input, h_input, N * sizeof(int), cudaMemcpyHostToDevice));

    // 启动 Kernel
    // 共享内存大小：每个 Block 需要 THREADS_PER_BLOCK * sizeof(int) 的空间
    size_t shared_mem_size = THREADS_PER_BLOCK * sizeof(int); 
    
    // 启动 Kernel，注意第三个参数是动态分配的共享内存大小
    printf("blocks: %d, threads: %d, sharemem: %lu.\n", NUM_BLOCKS, THREADS_PER_BLOCK, shared_mem_size);
    reduce_sum_shared<<<NUM_BLOCKS, THREADS_PER_BLOCK, shared_mem_size>>>(
        d_input, d_output, N
    );
    CUDA_CHECK(cudaGetLastError()); // 检查核函数启动是否成功

    // 等待设备完成计算
    CUDA_CHECK(cudaDeviceSynchronize());

    // 将部分结果（每个 Block 的求和结果）从设备传输回主机
    int* h_output = (int*)malloc(NUM_BLOCKS * sizeof(int));
    CUDA_CHECK(cudaMemcpy(h_output, d_output, NUM_BLOCKS * sizeof(int), cudaMemcpyDeviceToHost));

    // 在主机上对 Block 结果进行最终求和
    long long final_sum = 0;
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        final_sum += h_output[i];
    }

    printf("Input size N = %d\n", N);
    printf("Expected Sum = %d\n", N);
    printf("Calculated Sum = %lld\n", final_sum);

    // 清理资源
    free(h_input);
    free(h_output);
    CUDA_CHECK(cudaFree(d_input));
    CUDA_CHECK(cudaFree(d_output));

    return 0;
}
