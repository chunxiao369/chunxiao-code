#include <stdio.h>
#include <xmmintrin.h>  // SSE intrinsics
#include <immintrin.h>  // SSE intrinsics

#define SIZE 4
int main() {
    float a[SIZE] = {1.0, 2.0, 3.0, 4.0};
    float b[SIZE] = {5.0, 6.0, 7.0, 8.0};
    float result[SIZE];

    // 加载向量到SIMD寄存器（__m128是128位向量，容纳4个float）
    __m128 vec_a = _mm_loadu_ps(a);
    __m128 vec_b = _mm_loadu_ps(b);

    // SIMD加法：一条指令处理4个元素
    __m128 vec_result = _mm_add_ps(vec_a, vec_b);

    // 存储结果
    _mm_storeu_ps(result, vec_result);

    // 输出
    for (int i = 0; i < SIZE; i++) {
        printf("Result[%d] = %f\n", i, result[i]);
    }

    return 0;
}
