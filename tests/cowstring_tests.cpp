#include <gtest/gtest.h>
#include <replusplus/cowstring.hpp>

TEST(COWStringTests, DefaultCtor) { replusplus::COWString a{}; }

TEST(COWStringTests, CStrCtor) {
    replusplus::COWString a{"hello world!"};

    for (std::size_t i = 0; i < a.size(); i++) {
        printf("%c", a[i]);
    }
    printf("\n");

    EXPECT_STREQ(a.c_str(), "hello world!");
}

// TEST(COWStringTests, OutOfScope) {
//     {
//         replusplus::COWString a {"hi world!"};
//     }

// }