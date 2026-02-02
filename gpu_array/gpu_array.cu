#include "gpu_array.hpp"
#include <gtest/gtest.h>

TEST(GpuArrayPageable, Ctor) {
    fun::gpu_array<float, 10> arr;
    EXPECT_EQ(arr.size(), 10);
}
TEST(GpuArrayPageable, Access) {
    fun::gpu_array<float, 10> arr;
    EXPECT_EQ(arr.size(), 10);
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = 1.0f * i;
    }
    for (size_t i = 0; i < arr.size(); i++) {
        EXPECT_EQ(arr[i], 1.0f * i);
    }
}

TEST(GpuArrayPinned, Ctor) {
    fun::gpu_array<float, 10, true> arr;
    EXPECT_EQ(arr.size(), 10);
}

TEST(GpuArrayPinned, Access) {
    fun::gpu_array<float, 10, true> arr;
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = 2.0f * i;
    }
    for (size_t i = 0; i < arr.size(); i++) {
        EXPECT_EQ(arr[i], 2.0f * i);
    }
}

TEST(GpuArrayPageable, BoundsCheck) {
    fun::gpu_array<int, 5> arr;
    EXPECT_NO_THROW(arr.at(0));
    EXPECT_NO_THROW(arr.at(4));
    EXPECT_THROW(arr.at(5), std::out_of_range);
    EXPECT_THROW(arr.at(100), std::out_of_range);
}

TEST(GpuArrayPinned, BoundsCheck) {
    fun::gpu_array<int, 5, true> arr;
    EXPECT_NO_THROW(arr.at(0));
    EXPECT_NO_THROW(arr.at(4));
    EXPECT_THROW(arr.at(5), std::out_of_range);
    EXPECT_THROW(arr.at(100), std::out_of_range);
}

TEST(GpuArrayPageable, TransferRoundTrip) {
    fun::gpu_array<float, 100> arr;

    // Fill with known pattern
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = static_cast<float>(i) * 1.5f;
    }

    // Transfer to device and back
    arr.to_device();
    arr.to_host();

    // Verify data unchanged
    for (size_t i = 0; i < arr.size(); i++) {
        EXPECT_FLOAT_EQ(arr[i], static_cast<float>(i) * 1.5f);
    }
}

TEST(GpuArrayPinned, TransferRoundTrip) {
    fun::gpu_array<float, 100, true> arr;

    // Fill with known pattern
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = static_cast<float>(i) * 2.5f;
    }

    // Transfer to device and back
    arr.to_device();
    arr.to_host();

    // Verify data unchanged
    for (size_t i = 0; i < arr.size(); i++) {
        EXPECT_FLOAT_EQ(arr[i], static_cast<float>(i) * 2.5f);
    }
}

TEST(GpuArrayTests, MoveSemantics) {
    fun::gpu_array<int, 10> arr;

    // Fill original
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = static_cast<int>(i * 10);
    }

    // Move construct
    fun::gpu_array<int, 10> arr2(std::move(arr));

    // Verify new array has data
    EXPECT_EQ(arr2.size(), 10);
    for (size_t i = 0; i < arr2.size(); i++) {
        EXPECT_EQ(arr2[i], static_cast<int>(i * 10));
    }

    // Original should have nulled pointers (can't easily test this from outside)
    // But destructor shouldn't crash
}

__global__ void double_kernel(float* data, size_t n) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= n) {
        return;
    }

    data[tid] *= 2.0f;
}

TEST(GpuArrayTests, KernelExecution) {
    fun::gpu_array<float, 1024> arr;
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = static_cast<float>(i);
    }

    const dim3 threads_per_blk = 256;
    // int(ceil(n / tpb)
    const dim3 blks_per_grid = static_cast<int>(
        std::ceil(static_cast<float>(arr.size()) / threads_per_blk.x));

    arr.to_device();
    double_kernel<<<blks_per_grid, threads_per_blk>>>(arr.device_ptr(),
                                                      arr.size());
    cudaTry(cudaDeviceSynchronize());
    arr.to_host();

    for (size_t i = 0; i < arr.size(); i++) {
        ASSERT_FLOAT_EQ(arr[i], static_cast<float>(i) * 2.0f);
    }
}
