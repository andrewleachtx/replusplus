#include <replusplus/optional.hpp>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

TEST(OptionalTests, DefaultCtor) {
    replusplus::optional<float> maybe_float;
    EXPECT_FALSE(maybe_float.has_value());
    EXPECT_FALSE(static_cast<bool>(maybe_float));
}

TEST(OptionalTests, ValueCtor) {
    replusplus::optional<float> maybe_float{6.7f};
    EXPECT_TRUE(maybe_float.has_value());
    EXPECT_EQ(maybe_float.value(), 6.7f);
    EXPECT_EQ(*maybe_float, 6.7f);
    EXPECT_TRUE(static_cast<bool>(maybe_float));
}

TEST(OptionalTests, CopyCtor) {
    replusplus::optional<float> a{6.7f};
    replusplus::optional<float> b{a};

    EXPECT_EQ(a.has_value(), b.has_value());
    EXPECT_EQ(*a, *b);
    EXPECT_EQ(*b, 6.7f);
}

TEST(OptionalTests, CopyAssign) {
    replusplus::optional<float> a{6.7f};
    replusplus::optional<float> b;

    EXPECT_FALSE(b.has_value());
    b = a;
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(*b, 6.7f);
    EXPECT_EQ(*a, *b);
}

TEST(OptionalTests, CopyAssignEmpty) {
    replusplus::optional<int> a;
    replusplus::optional<int> b{42};

    b = a;
    EXPECT_FALSE(b.has_value());
}

TEST(OptionalTests, ValueAssign) {
    replusplus::optional<float> a{6.7f};
    replusplus::optional<float> b;

    EXPECT_FALSE(b.has_value());
    b = 3.7f;
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(*b, 3.7f);
    EXPECT_NE(*b, *a);
}

TEST(OptionalTests, ValueThrowsWhenEmpty) {
    replusplus::optional<int> empty;
    EXPECT_THROW(empty.value(), std::logic_error);
}

TEST(OptionalTests, MoveCtor) {
    replusplus::optional<std::string> a{"hello"};
    replusplus::optional<std::string> b{std::move(a)};

    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(*b, "hello");
}

TEST(OptionalTests, MoveAssign) {
    replusplus::optional<std::string> a{"world"};
    replusplus::optional<std::string> b{"hello"};

    b = std::move(a);
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(*b, "world");
}

TEST(OptionalTests, BoolConversion) {
    replusplus::optional<int> empty;
    replusplus::optional<int> full{42};

    if (empty) {
        FAIL() << "Empty optional evaluated to true";
    }

    if (full) {
        SUCCEED();
    }
    else {
        FAIL() << "Full optional evaluated to false";
    }
}
