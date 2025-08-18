// avoid definition redundancies
#ifndef KERNEL_CUH_
#define KERNEL_CUH_

#ifdef __cplusplus
extern "C" {
#endif

void asum(const float* A, const float* B, float* C, int n_el);

#ifdef __cplusplus
}
#endif

#endif

