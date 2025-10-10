#include <stdio.h>
#include <cuda_runtime.h>

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
// 映射函数：根据计算能力（Compute Capability）获取每个SM的CUDA核心数
// -------------------------------------------------------------
// 这是一个必须的映射表，因为不同架构的SM包含的CUDA Core数量不同
int _ConvertSMVer2Cores(int major, int minor) {
    // 资料来源：NVIDIA CUDA C Programming Guide / GPU Architecture
    // 查阅官方文档可以获得最新的和更完整的列表
    
    // (SM version) -> (Cores per SM)
    const int cores_per_sm[] = {
        /* 2.0 (Fermi) */ 32,
        /* 3.0 (Kepler) */ 192,
        /* 5.0 (Maxwell) */ 128,
        /* 6.0 (Pascal) */ 64,
        /* 7.0 (Volta) */ 64,
        /* 7.5 (Turing) */ 64,
        /* 8.0 (Ampere) */ 128,
        /* 8.6 (Ampere) */ 128, // RTX 30系列等
        /* 8.9 (Ada Lovelace) */ 128, // RTX 40系列等
        /* 9.0 (Hopper) */ 128  // H100等
    };

    if (major == 2 && minor == 0) return cores_per_sm[0];
    if (major == 3 && minor == 0) return cores_per_sm[1];
    if (major == 5 && minor == 0) return cores_per_sm[2];
    if (major == 6 && minor == 0) return cores_per_sm[3];
    if (major == 7 && minor == 0) return cores_per_sm[4];
    if (major == 7 && minor == 5) return cores_per_sm[5];
    if (major == 8 && minor == 0) return cores_per_sm[6];
    if (major == 8 && minor == 6) return cores_per_sm[7];
    if (major == 8 && minor == 9) return cores_per_sm[8];
    if (major == 9 && minor == 0) return cores_per_sm[9];

    // Fallback for unknown/future architectures
    fprintf(stderr, "Warning: Unknown SM version %d.%d. Unable to calculate core count.\n", major, minor);
    return 0; 
}


void get_cuda_core_count(int device) {
    cudaDeviceProp deviceProp;

    // 1. 获取设备属性
    CUDA_CHECK(cudaGetDeviceProperties(&deviceProp, device));

    // 2. 获取 SM 数量
    int sm_count = deviceProp.multiProcessorCount;
    
    // 3. 根据计算能力获取每个 SM 的核心数
    int cores_per_sm = _ConvertSMVer2Cores(deviceProp.major, deviceProp.minor);

    // 4. 计算总的 CUDA Core 数量
    int total_cores = sm_count * cores_per_sm;

    printf("\n--- GPU Device %d: %s ---\n", device, deviceProp.name);
    printf("Compute Capability: %d.%d\n", deviceProp.major, deviceProp.minor);
    printf("Streaming Multiprocessors (SM) Count: %d\n", sm_count);
    printf("CUDA Cores Per SM: %d\n", cores_per_sm);
    printf("Total CUDA Cores: %d\n", total_cores);
}

int main() {
    int device_count;
    
    // 获取设备数量
    CUDA_CHECK(cudaGetDeviceCount(&device_count));

    if (device_count == 0) {
        printf("No CUDA devices found.\n");
        return 0;
    }

    printf("Found %d CUDA devices.\n", device_count);

    // 遍历所有 GPU
    for (int i = 0; i < device_count; ++i) {
        get_cuda_core_count(i);
    }

    return 0;
}
