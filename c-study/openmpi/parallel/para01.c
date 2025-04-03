#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 8  // 矩阵大小 (为了示例简单，使用小的固定值)

void data_parallel_example1(int rank, int size) {
    double A[N][N], x[N], y[N];
    double local_y[N] = {0};
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;

    // 初始化矩阵和向量 (仅在 rank 0)
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            x[i] = 1.0;  // 测试用简单值
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j;  // 测试用简单值
            }
        }
    }

    // 广播向量 x 给所有进程
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // 分发矩阵的行
    double local_A[rows_per_process][N];
    MPI_Scatter(A, rows_per_process * N, MPI_DOUBLE,
                local_A, rows_per_process * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // 每个进程计算自己的部分结果
    // N为8，size为4时，每个单元算2行
    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < N; j++) {
            local_y[i] += local_A[i][j] * x[j];
        }
    }

    // 收集所有结果
    MPI_Gather(local_y, rows_per_process, MPI_DOUBLE,
              y, rows_per_process, MPI_DOUBLE,
              0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Data Parallel example1 Result (first few elements):\n");
        for (int i = 0; i < N; i++) {
            printf("%.1f ", y[i]);
        }
        printf("\n");
    }
}

void data_parallel_example2(int rank, int size) {
    double A[N][N], x[N], y[N] = {0};
    int cols_per_process = N / size;
    int start_col = rank * cols_per_process;
    int end_col = start_col + cols_per_process;

    // 初始化矩阵和向量 (仅在 rank 0)
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            x[i] = 1.0;
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j;
            }
        }
    }

    // 广播整个矩阵和向量
    MPI_Bcast(A, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // 每个进程计算部分列的乘法
    double local_sum[N] = {0};
    for (int i = 0; i < N; i++) {
        for (int j = start_col; j < end_col; j++) {
            local_sum[i] += A[i][j] * x[j];
        }
    }

    // 使用 Reduce 收集所有部分和
    MPI_Reduce(local_sum, y, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Data Parallel example2 Result (first few elements):\n");
        for (int i = 0; i < N; i++) {
            printf("%.1f ", y[i]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            printf("Matrix size must be divisible by number of processes\n");
        }
        MPI_Finalize();
        return 1;
    }

    printf("Process %d of %d\n", rank, size);

    // 数据并行示例
    data_parallel_example1(rank, size);
    MPI_Barrier(MPI_COMM_WORLD);

    // 模型并行示例
    data_parallel_example2(rank, size);

    MPI_Finalize();
    return 0;
}
