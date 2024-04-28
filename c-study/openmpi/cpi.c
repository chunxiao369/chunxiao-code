/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f(double);

double f(double a)
{
    return (4.0 / (1.0 + a * a));
}

#define MAX_NUM 10
int main(int argc, char *argv[])
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    double startwtime = 0.0, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double my_d[MAX_NUM], total_d[MAX_NUM];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    fprintf(stdout, "Process %d of %d is on %s\n", myid, numprocs, processor_name);
    fflush(stdout);

    n = 10000;  /* default # of rectangles */
    if (myid == 0)
        startwtime = MPI_Wtime();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = 1.0 / (double) n;
    sum = 0.0;
    /* A slightly better approach starts from large i and works back */
    for (i = myid + 1; i <= n; i += numprocs) {
        x = h * ((double) i - 0.5);
        sum += f(x);
    }
    mypi = h * sum;
    for (i = 0; i < MAX_NUM; i++) {
        my_d[i] = (double)1/MAX_NUM;
    }
#if 0
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        endwtime = MPI_Wtime();
        printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        printf("wall clock time = %f\n", endwtime - startwtime);
        fflush(stdout);
    }
    //fprintf(stdout, "Process %d of %d is on %s, mypi: %.16f\n", myid, numprocs, processor_name, mypi);
    //fflush(stdout);
    MPI_Reduce(my_d, total_d, MAX_NUM, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (myid == 1) {
        fprintf(stdout, "\n in id: %d my d: ", myid);
        for (i = 0; i < MAX_NUM; i++) {
            fprintf(stdout, "%.16f ", my_d[i]);
	}
        fprintf(stdout, "\ntotal d: ");
        for (i = 0; i < MAX_NUM; i++) {
            fprintf(stdout, "%.16f ", total_d[i]);
	}
        fprintf(stdout, "\n");
        fflush(stdout);
    }
#endif
    MPI_Allreduce(my_d, total_d, MAX_NUM, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if (myid == 1) {
        fprintf(stdout, "\n in id: %d my d: ", myid);
        for (i = 0; i < MAX_NUM; i++) {
            fprintf(stdout, "%.16f ", my_d[i]);
	}
        fprintf(stdout, "\ntotal d: ");
        for (i = 0; i < MAX_NUM; i++) {
            fprintf(stdout, "%.16f ", total_d[i]);
	}
        fprintf(stdout, "\n");
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}

