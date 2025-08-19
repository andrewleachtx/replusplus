#include "WeakPtr.h"
#include "SharedPtr.h"

#include <gtest/gtest.h>

template<typename T> class SharedPtrTypedTests : public ::testing::Test {};

using TestTypes = ::testing::Types<int, float, double, bool, std::vector<float>, std::vector<std::vector<float>>>;

TEST(WeakPtrTests, WeakPtrBreaksSharedCycle) {
    struct Node {
        SharedPtr<Node> child;
    };
}