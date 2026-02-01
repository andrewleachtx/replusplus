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

TEST(VectorTests, CopyAssign) {
    fun::vector<int> v = {1, 2, 3};
    fun::vector<int> v2 = {10, 20};

    v2 = v;

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTests, CopyAssignSelf) {
    fun::vector<int> v = {1, 2, 3};

    v = v;

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTests, CopyCtor) {
    fun::vector<int> v = {1, 2, 3};
    fun::vector<int> v2(v);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.capacity(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTests, ElementAccess) {
    fun::vector<int> v = {1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTests, ElementAccessAt) {
    fun::vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
}
