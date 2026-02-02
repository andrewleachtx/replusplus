#include "fun_macros.h"
#include <cstddef>
#include <cuda_runtime.h>
#include <stdexcept>
#include <type_traits>

/*
    A fixed-size GPU buffer that manages both host and device memory.

    The goal is to make setup and sync between the two easier.

    Unlike std::array, which is N * sizeof(T), the fixed buffer is heap
    allocated on host if you decide to make a very massive buffer.

    Also, this (can) support pinned host memory for faster DMA transfer:
    https://developer.nvidia.com/blog/how-optimize-data-transfers-cuda-cc/

    Size is compile-time (N) to motivate one thing: no reallocation cost.
*/
namespace fun {
// TODO(A): should we use pinnedmemory true by default?
template <typename T, size_t N, bool DoPinnedTransfer = false> class gpu_array {
public:
    gpu_array() : host_data_{}, device_data_{} {
        static_assert(std::is_trivially_copyable_v<T>,
                      "gpu_array only supports copying trivial types!");
        cudaTry(cudaMalloc(&device_data_, N * sizeof(T)));

        if constexpr (DoPinnedTransfer) {
            cudaTry(cudaMallocHost(&host_data_, N * sizeof(T)));
        }
        else {
            host_data_ = raw_alloc_arr(N);
        }
    }
    ~gpu_array() {
        cudaTry(cudaFree(device_data_));

        if constexpr (DoPinnedTransfer) {
            cudaTry(cudaFreeHost(host_data_));
        }
        else {
            delete_arr(host_data_);
        }
    }

    void to_device() {}
    void to_host() {}

    static constexpr size_t size() { return N; }

    T& operator[](size_t idx) { return host_data_[idx]; }
    const T& operator[](size_t idx) const { return host_data_[idx]; }
    T& at(size_t idx) {
        if (idx >= N) {
            throw std::out_of_range(".at(...) access out of range!");
        }

        return host_data_[idx];
    }
    const T& at(size_t idx) const {
        if (idx >= N) {
            throw std::out_of_range(".at(...) access out of range!");
        }

        return host_data_[idx];
    }
    T* device_ptr() const { return device_data_; }

private:
    T* host_data_;
    T* device_data_;

    static T* raw_alloc_arr(size_t desired_capacity) {
        T* arr =
            static_cast<T*>(::operator new[](desired_capacity * sizeof(T)));
        return arr;
    }
    static void delete_arr(T* arr) { ::operator delete[](arr); }
};
}; // namespace fun
