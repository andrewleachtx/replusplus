#include <gtest/gtest.h>
#include <numeric>
#include <replusplus/vector.hpp>

TEST(VectorTests, Ctor) {
    replusplus::vector<int> v;
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTests, CTAD) {
    replusplus::vector v = {6.7f};
    EXPECT_EQ(v.size(), 1);
}

TEST(VectorTests, CopyAssign) {
    replusplus::vector<int> v = {1, 2, 3};
    replusplus::vector<int> v2 = {10, 20};

    v2 = v;

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTests, CopyAssignSelf) {
    replusplus::vector<int> v = {1, 2, 3};

    v = v;

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTests, CopyCtor) {
    replusplus::vector<int> v = {1, 2, 3};
    replusplus::vector<int> v2(v);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.capacity(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTests, ElementAccess) {
    replusplus::vector<int> v = {1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTests, ElementAccessAt) {
    replusplus::vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
}

TEST(VectorTests, MoveAssign) {
    replusplus::vector<float> v = {6.0f, 7.0f};
    replusplus::vector<float> v2 = {4.2f, 3.0f};

    v2 = std::move(v);

    // v2 now has v's data
    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 6.0f);
    EXPECT_EQ(v2[1], 7.0f);

    // v should be in a valid but empty state
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTests, MoveCtor) {
    replusplus::vector<float> v = {6.0f, 7.0f};

    replusplus::vector<float> v2(std::move(v));

    // v2 stole v's data
    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 6.0f);
    EXPECT_EQ(v2[1], 7.0f);

    // v should be in a valid but empty state
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTests, PushBack) {
    replusplus::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
    EXPECT_GE(v.capacity(), 3);
}

TEST(VectorTests, PushBackMove) {
    replusplus::vector<std::string> v;
    std::string s = "hello";
    v.push_back(std::move(s));

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "hello");

    // s was moved from — should be empty
    EXPECT_TRUE(s.empty());
}

TEST(VectorTests, ResizeGrowsWithDefaultValues) {
    replusplus::vector<int> v;

    v.resize(5);

    ASSERT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);

    for (const int value : v) {
        EXPECT_EQ(value, 0);
    }
}

TEST(VectorTests, ResizeGrowsWithProvidedValue) {
    replusplus::vector<int> v = {1, 2, 3};

    v.resize(6, 67);

    ASSERT_EQ(v.size(), 6);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 67);
    EXPECT_EQ(v[4], 67);
    EXPECT_EQ(v[5], 67);
}

TEST(VectorTests, ResizeShrinksWithoutChangingSurvivors) {
    replusplus::vector<int> v = {1, 2, 3, 4, 5};
    const auto old_capacity = v.capacity();

    v.resize(3, 67);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTests, ResizeToSameSizeDoesNothing) {
    replusplus::vector<int> v = {1, 2, 3};
    const auto old_capacity = v.capacity();
    const auto old_data = v.data();

    v.resize(3, 67);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v.data(), old_data);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTests, ResizeToZero) {
    replusplus::vector<int> v = {1, 2, 3};
    const auto old_capacity = v.capacity();

    v.resize(0);

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.capacity(), old_capacity);
}

TEST(VectorTests, Comparison) {
    replusplus::vector<int> a, b;

    EXPECT_EQ(a, b);

    a.resize(3, -1);
    EXPECT_NE(a, b);

    b.resize(3, -1);
    EXPECT_EQ(a, b);

    a[0] = 0;
    EXPECT_NE(a, b);

    // {1, 2, 3} should be lte, gte {1, 2, 3}
    std::iota(a.begin(), a.end(), 1);
    std::iota(b.begin(), b.end(), 1);

    EXPECT_LE(a, b);
    EXPECT_GE(a, b);

    // and {1, 2} should be lt {1, 2, 3} and {1, 2, 3} gt {1, 2}.
    a.resize(2);
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
}
