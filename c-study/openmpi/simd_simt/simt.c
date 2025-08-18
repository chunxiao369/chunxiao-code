#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4  // 模拟一个warp的大小

// 线程函数：每个线程执行相同指令（加法），但数据独立
void* thread_add(void* arg) {
    int thread_id = *(int*)arg;
    float a = thread_id + 1.0;  // 模拟独立数据
    float b = thread_id + 5.0;
    float* result = malloc(sizeof(float));
    *result = a + b;  // 相同指令：加法
    printf("Thread %d: Result = %f\n", thread_id, *result);
    return result;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    void* results[NUM_THREADS];

    // 创建线程：模拟SIMT的并行执行
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_add, &thread_ids[i]);
    }

    // 等待线程完成（类似于warp同步）
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &results[i]);
        // 这里可以收集结果，但为了简单，仅打印
    }

    return 0;
}
