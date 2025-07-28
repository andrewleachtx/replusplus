#include "UniquePtr.h"

#include <gtest/gtest.h>

template <typename T>
class SharedPtrTypedTests : public ::testing::Test {};

using TestTypes = ::testing::Types<int, float, double, bool, std::vector<float>, std::vector<std::vector<float>>>;

TYPED_TEST_SUITE(SharedPtrTypedTests, TestTypes);

TYPED_TEST(SharedPtrTypedTests, DefaultConstructorTest) {
    UniquePtr<TypeParam> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_FALSE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, RawPtrConstructorTest) {
    auto *rawPtr = new TypeParam();
    UniquePtr<TypeParam> ptr(rawPtr);
    EXPECT_EQ(ptr.get(), rawPtr);
    EXPECT_TRUE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, MoveConstructorTest) {
    UniquePtr<TypeParam> src(new TypeParam);
    UniquePtr<TypeParam> dst(std::move(src));
    EXPECT_FALSE(src);
    EXPECT_TRUE(dst);
}

TYPED_TEST(SharedPtrTypedTests, DestructorTest) {
    EXPECT_NO_THROW({
        UniquePtr<TypeParam> p(new TypeParam);
    });
}

TYPED_TEST(SharedPtrTypedTests, CopyOperatorTest) {
    constexpr bool isCopyAssignable = std::is_copy_assignable_v<UniquePtr<TypeParam> >;
    
    static_assert(!isCopyAssignable, "Copy assignment should be deleted!");
}

TYPED_TEST(SharedPtrTypedTests, MoveCopyOperatorTest) {
    UniquePtr<TypeParam> a(new TypeParam());
    UniquePtr<TypeParam> b;
    b = std::move(a);
    EXPECT_FALSE(a);
    EXPECT_TRUE(b);
}

TYPED_TEST(SharedPtrTypedTests, MoveCopyOperatorSelfAssignmentTest) {
    UniquePtr<TypeParam> ptr(new TypeParam());
    EXPECT_NO_THROW(
        ptr = std::move(ptr);
    );
    EXPECT_TRUE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, ReleaseTest) {
    auto *rawPtr = new TypeParam();
    UniquePtr<TypeParam> ptr(rawPtr);
    auto *got = ptr.release();
    EXPECT_EQ(got, rawPtr);
    EXPECT_FALSE(ptr);

    // The caller is supposed to handle cleanup https://en.cppreference.com/w/cpp/memory/unique_ptr/release.html
    delete rawPtr;
}

TYPED_TEST(SharedPtrTypedTests, ResetTest) {
    auto *rawPtr1 = new TypeParam();
    auto *rawPtr2 = new TypeParam();

    UniquePtr<TypeParam> ptr(rawPtr1);
    ptr.reset(rawPtr2);
    EXPECT_EQ(ptr.get(), rawPtr2);

    ptr.reset();
    EXPECT_FALSE(ptr);
}

TYPED_TEST(SharedPtrTypedTests, SwapTest) {
    auto *rawPtrA = new TypeParam();
    auto *rawPtrB = new TypeParam();
    UniquePtr<TypeParam> a(rawPtrA);
    UniquePtr<TypeParam> b(rawPtrB);

    a.swap(b);
    EXPECT_EQ(a.get(), rawPtrB);
    EXPECT_EQ(b.get(), rawPtrA);
}

TYPED_TEST(SharedPtrTypedTests, GetTest) {
    auto *rawPtr = new TypeParam();
    UniquePtr<TypeParam> ptr(rawPtr);

    EXPECT_EQ(ptr.get(), rawPtr);
}

TYPED_TEST(SharedPtrTypedTests, BoolOperatorTest) {
    UniquePtr<TypeParam> a;
    EXPECT_FALSE(a);

    a = UniquePtr<TypeParam>(new TypeParam());
    EXPECT_TRUE(a);
}

TYPED_TEST(SharedPtrTypedTests, DereferenceAndArrowTest) {
    UniquePtr<TypeParam> ptr(new TypeParam());

    EXPECT_NO_THROW({
        TypeParam &ref = *ptr;
        (void)ref;
    });

    if constexpr (std::is_class_v<TypeParam>) {
        EXPECT_EQ(ptr->size(), TypeParam().size());
    }
}

TYPED_TEST(SharedPtrTypedTests, MakeUniqueTest) {
    auto ptr = MakeUnique<TypeParam>();

    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, TypeParam());
}

TYPED_TEST(SharedPtrTypedTests, NullptrConstructorAndReleaseTest) {
    // Note we are not calling default but passing in nullptr
    UniquePtr<TypeParam> p(nullptr);
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);

    auto *got = p.release();
    EXPECT_EQ(got, nullptr);
    EXPECT_FALSE(p);
}

TYPED_TEST(SharedPtrTypedTests, ResetEmptyDoesNothing) {
    UniquePtr<TypeParam> p;
    EXPECT_NO_THROW(p.reset());
    EXPECT_FALSE(p);
}

TYPED_TEST(SharedPtrTypedTests, SelfSwapAllowed) {
    auto *raw = new TypeParam();
    UniquePtr<TypeParam> p(raw);
    EXPECT_NO_THROW(p.swap(p));
    EXPECT_EQ(p.get(), raw);
}

TYPED_TEST(SharedPtrTypedTests, SwapTwoEmpties) {
    UniquePtr<TypeParam> a, b;
    EXPECT_NO_THROW(a.swap(b));
    EXPECT_FALSE(a);
    EXPECT_FALSE(b);
}