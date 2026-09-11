#include <replusplus/vector.hpp>
#include <gtest/gtest.h>

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
