#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <cuda_runtime.h>
#include <nccl.h>

#define CUDA_CALL(cmd) do {                        \
    cudaError_t e = cmd;                           \
    if (e != cudaSuccess) {                        \
        printf("CUDA error %s:%d '%s'\n",          \
                __FILE__, __LINE__, cudaGetErrorString(e)); \
        exit(EXIT_FAILURE);                        \
    }                                              \
} while(0)

#define NCCL_CALL(cmd) do {                        \
    ncclResult_t r = cmd;                          \
    if (r != ncclSuccess) {                        \
        printf("NCCL error %s:%d '%s'\n",          \
                __FILE__, __LINE__, ncclGetErrorString(r)); \
        exit(EXIT_FAILURE);                        \
    }                                              \
} while(0)

struct ThreadArgs {
    int rank;
    int nGPUs;
    ncclComm_t comm;
    ncclUniqueId id;
};

void* run(void* arg) {
    struct ThreadArgs* targs = (struct ThreadArgs*)arg;
    int rank = targs->rank;
    int nGPUs = targs->nGPUs;

    // 绑定 GPU
    CUDA_CALL(cudaSetDevice(rank));

    // 初始化 NCCL
    ncclComm_t comm = targs->comm;
    //NCCL_CALL(ncclCommInitRank(&comm, nGPUs, targs->id, rank));

    // 创建 CUDA stream
    cudaStream_t stream;
    CUDA_CALL(cudaStreamCreate(&stream));

    // 分配内存
    float *sendbuff, *recvbuff;
    CUDA_CALL(cudaMalloc((void **)&sendbuff, sizeof(float)));
    CUDA_CALL(cudaMalloc((void **)&recvbuff, sizeof(float)));
    printf("[Rank %d] start ...\n", rank);

    float value = (float)(rank + 1); // rank0=1, rank1=2, ...
    CUDA_CALL(cudaMemcpy(sendbuff, &value, sizeof(float), cudaMemcpyHostToDevice));

    // AllReduce: sum
    NCCL_CALL(ncclAllReduce(sendbuff, recvbuff, 1, ncclFloat, ncclSum, comm, stream));

    // 等待 stream 完成
    CUDA_CALL(cudaStreamSynchronize(stream));

    CUDA_CALL(cudaMemcpy(&value, recvbuff, sizeof(float), cudaMemcpyDeviceToHost));

    printf("[Rank %d] Result = %f\n", rank, value);

    // 清理
    ncclCommDestroy(comm);
    cudaStreamDestroy(stream);
    cudaFree(sendbuff);
    cudaFree(recvbuff);

    return NULL;
}

int main() {
    int nGPUs = 2;  // 用几张 GPU
    ncclUniqueId id;
    pthread_t threads[nGPUs];
    struct ThreadArgs args[nGPUs];
    ncclComm_t comms[nGPUs];

    // 在main单一线程初始化所有comms
    NCCL_CALL(ncclCommInitAll(comms, nGPUs, NULL));  // NULL表示本地连续GPU (0 to nGPUs-1)

    // rank=0 生成唯一 NCCL id，线程共享
    NCCL_CALL(ncclGetUniqueId(&id));

    for (int i = 0; i < nGPUs; i++) {
        args[i].rank = i;
        args[i].nGPUs = nGPUs;
        args[i].id = id;
        args[i].comm = comms[i];
        pthread_create(&threads[i], NULL, run, (void*)&args[i]);
    }

    for (int i = 0; i < nGPUs; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

