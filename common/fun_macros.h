#pragma once
#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>

#define CUDA_TRY(ans)                                                       \
    { gpuAssert((ans), __FILE__, __LINE__); }

__inline __host__ void gpuAssert(cudaError_t code, const char* file, int line,
                                 bool abort = true) {
    if (code != cudaSuccess) {
        fprintf(stderr, "CUDA_TRY ASSERTION FAILED: %s %s %d\n",
                cudaGetErrorString(code), file, line);
        if (abort)
            exit(code);
    }
}
