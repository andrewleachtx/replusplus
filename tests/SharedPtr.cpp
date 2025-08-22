#include "SharedPtr.h"

#include <gtest/gtest.h>

template<typename T> class SharedPtrTypedTests : public ::testing::Test {};

using TestTypes = ::testing::Types<int, float, double, bool, std::vector<float>, std::vector<std::vector<float>>>;

TYPED_TEST_SUITE(SharedPtrTypedTests, TestTypes);

TYPED_TEST(SharedPtrTypedTests, DefaultConstructorTest) {
    SharedPtr<TypeParam> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0ULL);
    EXPECT_FALSE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, RawPtrConstructorTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> ptr(rawPtr);
    EXPECT_EQ(ptr.get(), rawPtr);
    EXPECT_EQ(ptr.use_count(), 1ULL);
    EXPECT_TRUE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, CopyConstructorTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> a(rawPtr);
    SharedPtr<TypeParam> b(a);
    EXPECT_EQ(a.get(), b.get());
    EXPECT_EQ(a.use_count(), 2ULL);
    EXPECT_EQ(b.use_count(), 2ULL);
}

TYPED_TEST(SharedPtrTypedTests, CopyOperatorTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> a(rawPtr);
    SharedPtr<TypeParam> b;
    b = a;
    EXPECT_EQ(a.get(), b.get());
    EXPECT_EQ(a.use_count(), 2ULL);
    EXPECT_EQ(b.use_count(), 2ULL);
}

TYPED_TEST(SharedPtrTypedTests, MoveConstructorTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> a(rawPtr);
    SharedPtr<TypeParam> b(std::move(a));
    EXPECT_EQ(b.get(), rawPtr);
    EXPECT_EQ(b.use_count(), 1ULL);
    EXPECT_TRUE(b);
    EXPECT_FALSE(a);
    EXPECT_EQ(a.use_count(), 0ULL);
}

TYPED_TEST(SharedPtrTypedTests, DestructorTest) {
    EXPECT_NO_THROW({ SharedPtr<TypeParam> p(new TypeParam); });
}

TYPED_TEST(SharedPtrTypedTests, MoveCopyOperatorTest) {
    SharedPtr<TypeParam> a(new TypeParam());
    SharedPtr<TypeParam> b;
    b = std::move(a);
    EXPECT_FALSE(a);
    EXPECT_TRUE(b);
}

TYPED_TEST(SharedPtrTypedTests, MoveCopyOperatorSelfAssignmentTest) {
    SharedPtr<TypeParam> ptr(new TypeParam());
    EXPECT_NO_THROW(ptr = std::move(ptr););
    EXPECT_TRUE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, ResetTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> ptr(rawPtr);

    ptr.reset();
    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.use_count(), 0ULL);
}

TYPED_TEST(SharedPtrTypedTests, ResetToPtrTest) {
    auto *raw = new TypeParam();
    SharedPtr<TypeParam> p1(raw);
    SharedPtr<TypeParam> p2 = p1;
    EXPECT_EQ(p1.use_count(), 2u);

    p1.reset();
    EXPECT_EQ(p1.use_count(), 0u);
    EXPECT_EQ(p2.use_count(), 1u);

    p2.reset();
    EXPECT_EQ(p2.use_count(), 0u);
}

TYPED_TEST(SharedPtrTypedTests, ResetToNullTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> ptr(rawPtr);
    EXPECT_EQ(ptr.use_count(), 1ULL);
    ptr.reset(nullptr);
    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.use_count(), 0ULL);
}

TYPED_TEST(SharedPtrTypedTests, SwapTest) {
    auto *rawPtrA = new TypeParam();
    auto *rawPtrB = new TypeParam();
    SharedPtr<TypeParam> a(rawPtrA);
    SharedPtr<TypeParam> b(rawPtrB);

    EXPECT_EQ(a.use_count(), 1ULL);
    EXPECT_EQ(b.use_count(), 1ULL);
    a.swap(b);
    EXPECT_EQ(a.get(), rawPtrB);
    EXPECT_EQ(b.get(), rawPtrA);
    EXPECT_EQ(a.use_count(), 1ULL);
    EXPECT_EQ(b.use_count(), 1ULL);
}

TYPED_TEST(SharedPtrTypedTests, GetTest) {
    auto *rawPtr = new TypeParam();
    SharedPtr<TypeParam> ptr(rawPtr);

    EXPECT_EQ(ptr.get(), rawPtr);
}

TYPED_TEST(SharedPtrTypedTests, BoolOperatorTest) {
    SharedPtr<TypeParam> a;
    EXPECT_FALSE(a);

    a = SharedPtr<TypeParam>(new TypeParam());
    EXPECT_TRUE(a);
}

TYPED_TEST(SharedPtrTypedTests, DereferenceAndArrowTest) {
    SharedPtr<TypeParam> ptr(new TypeParam());

    EXPECT_NO_THROW({
        TypeParam &ref = *ptr;
        (void)ref;
    });

    if constexpr (std::is_class_v<TypeParam>) {
        EXPECT_EQ(ptr->size(), TypeParam().size());
    }
}

TYPED_TEST(SharedPtrTypedTests, MakeSharedTest) {
    auto ptr = MakeShared<TypeParam>();

    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr.use_count(), 1ULL);
    EXPECT_EQ(*ptr, TypeParam());
}

TYPED_TEST(SharedPtrTypedTests, NullptrConstructorAndResetTest) {
    // Note we are not calling default but passing in nullptr
    SharedPtr<TypeParam> p(nullptr);
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);

    p.reset();
    EXPECT_FALSE(p);
}

TYPED_TEST(SharedPtrTypedTests, ResetEmptyDoesNothing) {
    SharedPtr<TypeParam> p;
    EXPECT_NO_THROW(p.reset());
    EXPECT_FALSE(p);
}

TYPED_TEST(SharedPtrTypedTests, SelfSwapAllowed) {
    auto *raw = new TypeParam();
    SharedPtr<TypeParam> p(raw);
    EXPECT_NO_THROW(p.swap(p));
    EXPECT_EQ(p.get(), raw);
}

TYPED_TEST(SharedPtrTypedTests, SwapTwoEmpties) {
    SharedPtr<TypeParam> a, b;
    EXPECT_NO_THROW(a.swap(b));
    EXPECT_FALSE(a);
    EXPECT_FALSE(b);
}
