#include "vector.hpp"
#include <gtest/gtest.h>

TEST(VectorTests, Ctor) {
    fun::vector<int> v;
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTests, CTAD) {
    fun::vector v = {6.7f};
    EXPECT_EQ(v.size(), 1);
}

TEST(VectorTests, Smoke) {
    fun::vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
}
 
TEST(VectorTests, ElementAccess) {
    fun::vector<int> v = {1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}
