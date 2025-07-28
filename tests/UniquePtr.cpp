#include "UniquePtr.h"

#include <gtest/gtest.h>

template <typename T>
class UniquePtrTypedTests : public ::testing::Test {};

using TestTypes = ::testing::Types<int, float, double, bool, std::vector<float>, std::vector<std::vector<float>>>;

TYPED_TEST_SUITE(UniquePtrTypedTests, TestTypes);

TYPED_TEST(UniquePtrTypedTests, DefaultConstructorTest) {
    UniquePtr<TypeParam> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_FALSE(ptr);
}

TYPED_TEST(UniquePtrTypedTests, RawPtrConstructorTest) {
    auto *rawPtr = new TypeParam();
    UniquePtr<TypeParam> ptr(rawPtr);
    EXPECT_EQ(ptr.get(), rawPtr);
    EXPECT_TRUE(ptr);
}

TYPED_TEST(UniquePtrTypedTests, MoveConstructorTest) {
    UniquePtr<TypeParam> src(new TypeParam);
    UniquePtr<TypeParam> dst(std::move(src));
    EXPECT_FALSE(src);
    EXPECT_TRUE(dst);
}

TYPED_TEST(UniquePtrTypedTests, DestructorTest) {
    EXPECT_NO_THROW({
        UniquePtr<TypeParam> p(new TypeParam);
    });
}

TYPED_TEST(UniquePtrTypedTests, CopyOperatorTest) {
    constexpr bool isCopyAssignable = std::is_copy_assignable_v<UniquePtr<TypeParam> >;
    
    static_assert(!isCopyAssignable, "Copy assignment should be deleted!");
}

TYPED_TEST(UniquePtrTypedTests, MoveCopyOperatorTest) {
    UniquePtr<TypeParam> a(new TypeParam());
    UniquePtr<TypeParam> b;
    b = std::move(a);
    EXPECT_FALSE(a);
    EXPECT_TRUE(b);
}

TYPED_TEST(UniquePtrTypedTests, MoveCopyOperatorSelfAssignmentTest) {
    UniquePtr<TypeParam> ptr(new TypeParam());
    EXPECT_NO_THROW(
        ptr = std::move(ptr);
    );
    EXPECT_TRUE(ptr);
}

TYPED_TEST(UniquePtrTypedTests, ReleaseTest) {
    auto *rawPtr = new TypeParam();
    UniquePtr<TypeParam> ptr(rawPtr);
    auto *got = ptr.release();
    EXPECT_EQ(got, rawPtr);
    EXPECT_FALSE(ptr);
}

TYPED_TEST(UniquePtrTypedTests, ResetTest) {
    auto *rawPtr1 = new TypeParam();
    auto *rawPtr2 = new TypeParam();

    UniquePtr<TypeParam> ptr(rawPtr1);
    ptr.reset(rawPtr2);
    EXPECT_EQ(ptr.get(), rawPtr2);

    ptr.reset();
    EXPECT_FALSE(ptr);
}

TYPED_TEST(UniquePtrTypedTests, SwapTest) {
    auto *rawPtrA = new TypeParam();
    auto *rawPtrB = new TypeParam();
    UniquePtr<TypeParam> a(rawPtrA);
    UniquePtr<TypeParam> b(rawPtrB);

    a.swap(b);
    EXPECT_EQ(a.get(), rawPtrB);
    EXPECT_EQ(b.get(), rawPtrA);
}

TYPED_TEST(UniquePtrTypedTests, GetTest) {
    auto *rawPtr = new TypeParam();
    UniquePtr<TypeParam> ptr(rawPtr);

    EXPECT_EQ(ptr.get(), rawPtr);
}

TYPED_TEST(UniquePtrTypedTests, BoolOperatorTest) {
    UniquePtr<TypeParam> a;
    EXPECT_FALSE(a);

    a = UniquePtr<TypeParam>(new TypeParam());
    EXPECT_TRUE(a);
}

TYPED_TEST(UniquePtrTypedTests, DereferenceAndArrowTest) {
    UniquePtr<TypeParam> ptr(new TypeParam());

    EXPECT_NO_THROW({
        TypeParam &ref = *ptr;
        (void)ref;
    });

    if constexpr (std::is_class_v<TypeParam>) {
        EXPECT_EQ(ptr->size(), TypeParam().size());
    }
}

TYPED_TEST(UniquePtrTypedTests, MakeUniqueTest) {
    auto ptr = MakeUnique<TypeParam>();

    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, TypeParam());
}