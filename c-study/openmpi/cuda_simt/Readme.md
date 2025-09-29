# compile cmd
nvcc  kernel.cu vector_addition.c -lcudart -lrt -o vec_add

./vec_add 
n_el: 8.
blocks: 1, threads: 8
dim: 8, block: 0, thread: 0.
dim: 8, block: 0, thread: 1.
dim: 8, block: 0, thread: 2.
dim: 8, block: 0, thread: 3.
dim: 8, block: 0, thread: 4.
dim: 8, block: 0, thread: 5.
dim: 8, block: 0, thread: 6.
dim: 8, block: 0, thread: 7.
A+B: 1.000000, C: 1.000000
A+B: 1.381773, C: 1.381773
A+B: 0.493151, C: 0.493151
A+B: -0.848872, C: -0.848872
A+B: -1.410446, C: -1.410446
A+B: -0.675262, C: -0.675262
A+B: 0.680755, C: 0.680755
A+B: 1.410889, C: 1.410889
err: 0.000000

软件层级（Thread/Block/Grid）是程序员可见的抽象，而硬件层级（Warp/share memory/Bank）决定了实际执行效率
Shared Memory（共享内存）每个 SM（流式多处理器）上的片上 SRAM，是线程块内线程共享的数据区域。
BANK 是Shared Memory 内部的硬件结构，用于实现并行访问。Shared Memory 被划分为 32 个 bank（与 warp size 对应），每个 bank 独立读写，位宽为 32 位（可存储一个 float 或 int）。当 warp 内的 32 个线程访问不同 bank 时，可一次完成（无冲突）
warp 内的线程执行相同的指令（SIMT 架构：单指令多线程）。如果线程因分支（如 if-else）执行不同指令，warp 会串行执行（分支分化，降低效率）。

// CUDA kernel: 高性能矩阵转置
#define WARP_SIZE 32
#define BLOCK_ROWS 8

__global__ void transpose(float *odata, const float *idata, int width, int height)
{
    __shared__ float tile[WARP_SIZE][WARP_SIZE + 1];  // 避免bank conflict

    int x = blockIdx.x * WARP_SIZE + threadIdx.x;
    int y = blockIdx.y * WARP_SIZE + threadIdx.y;

    // Coalesced global memory read
    for (int j = 0; j < WARP_SIZE; j += BLOCK_ROWS) {
        tile[threadIdx.y + j][threadIdx.x] = idata[(y + j) * width + x];
    }

    __syncthreads();

    x = blockIdx.y * WARP_SIZE + threadIdx.x;
    y = blockIdx.x * WARP_SIZE + threadIdx.y;

    // Coalesced global memory write
    for (int j = 0; j < WARP_SIZE; j += BLOCK_ROWS) {
        odata[(y + j) * height + x] = tile[threadIdx.x][threadIdx.y + j];
    }
}
nvcc cuda_demo.cu -lcudart -lrt -o cuda_demo
