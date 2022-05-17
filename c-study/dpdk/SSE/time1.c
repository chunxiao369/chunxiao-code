/*
 * glibc中的strstr已经使用了SSE优化
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <nmmintrin.h>

uint32_t sse_add(int *a, size_t n)
{
    assert(a);
    uint32_t sum = 0;
    __m128i sse_sum = _mm_setzero_si128();
    __m128i sse_load;
    __m128i *p = (__m128i *) a;

    size_t block = n / 4;       // SSE寄存器能一次处理4个32位的整数
    size_t reserve = n % 4;     // 剩余的不足16字节

    for (size_t i = 0; i < block; ++i) {
        sse_load = _mm_load_si128(p);
        sse_sum = _mm_add_epi32(sse_sum, sse_load); // 带符号32位紧缩加法
        ++p;
    }

    // 剩余的不足16字节
    uint32_t *q = (uint32_t *)p;
    for (size_t i = 0; i < reserve; ++i) {
        sum += q[i];
    }

    // 将累加值合并
    sse_sum = _mm_hadd_epi32(sse_sum, sse_sum); // 带符号32位水平加法
    sse_sum = _mm_hadd_epi32(sse_sum, sse_sum);

    sum += _mm_cvtsi128_si32(sse_sum);  // 返回低32位
    return sum;
}

uint32_t sse_add_loop4(int *a, int n)
{ 
    assert(a);

    uint32_t sum = 0;
    size_t block = n / 16;   // SSE寄存器能一次处理4个32位的整数
    size_t reserve = n % 16; // 剩余的字节

    __m128i sse_sum0 = _mm_setzero_si128();
    __m128i sse_sum1 = _mm_setzero_si128();
    __m128i sse_sum2 = _mm_setzero_si128();
    __m128i sse_sum3 = _mm_setzero_si128();
    __m128i sse_load0;
    __m128i sse_load1;
    __m128i sse_load2;
    __m128i sse_load3;
    __m128i *p = (__m128i*)a;

    for(size_t i = 0; i < block; ++i) {
        sse_load0 = _mm_load_si128(p);
        sse_load1 = _mm_load_si128(p+1);
        sse_load2 = _mm_load_si128(p+2);
        sse_load3 = _mm_load_si128(p+3);

        sse_sum0 = _mm_add_epi32(sse_sum0, sse_load0);
        sse_sum1 = _mm_add_epi32(sse_sum1, sse_load1);
        sse_sum2 = _mm_add_epi32(sse_sum2, sse_load2);
        sse_sum3 = _mm_add_epi32(sse_sum3, sse_load3);

        p += 4;
    }

    // 剩余的不足16字节
    uint32_t *q = (uint32_t *)p;
    for(size_t i = 0; i < reserve; ++i) {
        sum += q[i];
    }

    // 将累加值两两合并
    sse_sum0 = _mm_add_epi32(sse_sum0, sse_sum1);
    sse_sum2 = _mm_hadd_epi32(sse_sum2, sse_sum3);
    sse_sum0 = _mm_add_epi32(sse_sum0, sse_sum2);

    sse_sum0 = _mm_hadd_epi32(sse_sum0, sse_sum0);
    sse_sum0 = _mm_hadd_epi32(sse_sum0, sse_sum0);

    sum += _mm_cvtsi128_si32(sse_sum0); // 取低32位

    return sum;
}

int normal_add(int *a, size_t n)
{
    uint32_t sum = 0;
    uint32_t *p;
    p = (uint32_t *)a;
    for (size_t i = 0; i < n; ++i) {
        sum += p[i];
    }
    return sum;
}

#define TEST_NUM 10000000
int main(int argc, char *argv[])
{
    int *a = NULL;
    int sum = 0;
    struct timeval b;
    struct timeval e;
    struct timeval res;

    if (2 != argc) {
        printf("%s N/S\n", argv[0]);
        return 0;
    }
    posix_memalign((void**)&a, 16, sizeof(int) * TEST_NUM);

    for (size_t i = 0; i < TEST_NUM; ++i) {
        a[i] = 66;
    }
    gettimeofday(&b, NULL);
    if (strcmp("N", argv[1]) == 0) {
        sum = normal_add(a, TEST_NUM);
    } else {
        sum = sse_add_loop4(a, TEST_NUM);
        //sum = sse_add(a, TEST_NUM);
    }
    gettimeofday(&e, NULL);
    timersub(&e, &b, &res);
    printf("seconds: %lu, useconds: %lu, result: %d\n", res.tv_sec, res.tv_usec, sum);

    free(a);
    return 0;
}
