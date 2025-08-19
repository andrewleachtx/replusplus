#include "Vector.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

template<typename T> class VectorTypedTests : public ::testing::Test {};

using TestTypes = ::testing::Types<int, float, double, std::string, std::vector<int>>;

TYPED_TEST_SUITE(VectorTypedTests, TestTypes);

TYPED_TEST(VectorTypedTests, DefaultConstructorTest) {
    Vector<TypeParam> vec;
    EXPECT_EQ(vec.data(), nullptr);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TYPED_TEST(VectorTypedTests, CountConstructorTest) {
    const size_t count = 5;
    Vector<TypeParam> vec(count);
    EXPECT_NE(vec.data(), nullptr);
    EXPECT_EQ(vec.size(), count);
    EXPECT_EQ(vec.capacity(), count);

    for (size_t i = 0; i < count; ++i) {
        EXPECT_EQ(vec.data()[i], TypeParam{});
    }
}

TYPED_TEST(VectorTypedTests, CountConstructorZeroTest) {
    Vector<TypeParam> vec(0);
    EXPECT_NE(vec.data(), nullptr);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TYPED_TEST(VectorTypedTests, CountValueConstructorTest) {
    const size_t count = 3;
    TypeParam value{};
    if constexpr (std::is_arithmetic_v<TypeParam>) {
        value = static_cast<TypeParam>(42);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
        value = "test";
    }

    Vector<TypeParam> vec(count, value);
    EXPECT_NE(vec.data(), nullptr);
    EXPECT_EQ(vec.size(), count);
    EXPECT_EQ(vec.capacity(), count);

    for (size_t i = 0; i < count; ++i) {
        EXPECT_EQ(vec.data()[i], value);
    }
}

TYPED_TEST(VectorTypedTests, CopyConstructorTest) {
    const size_t count = 4;
    Vector<TypeParam> original(count);

    Vector<TypeParam> copy(original);
    EXPECT_NE(copy.m_data, nullptr);
    EXPECT_NE(copy.m_data, original.m_data);
    EXPECT_EQ(copy.m_size, original.m_size);
    EXPECT_EQ(copy.m_capacity, original.m_capacity);

    for (size_t i = 0; i < count; ++i) {
        EXPECT_EQ(copy.m_data[i], original.m_data[i]);
    }
}

TYPED_TEST(VectorTypedTests, CopyConstructorEmptyTest) {
    Vector<TypeParam> original;
    Vector<TypeParam> copy(original);

    EXPECT_EQ(copy.m_data, nullptr);
    EXPECT_EQ(copy.m_size, 0);
    EXPECT_EQ(copy.m_capacity, 0);
}

TYPED_TEST(VectorTypedTests, MoveConstructorTest) {
    const size_t count = 4;
    Vector<TypeParam> original(count);
    auto original_data = original.m_data;
    auto original_size = original.m_size;
    auto original_capacity = original.m_capacity;

    Vector<TypeParam> moved(std::move(original));

    EXPECT_EQ(moved.m_data, original_data);
    EXPECT_EQ(moved.m_size, original_size);
    EXPECT_EQ(moved.m_capacity, original_capacity);

    EXPECT_EQ(original.m_data, nullptr);
    EXPECT_EQ(original.m_size, 0);
    EXPECT_EQ(original.m_capacity, 0);
}

TYPED_TEST(VectorTypedTests, MoveConstructorEmptyTest) {
    Vector<TypeParam> original;
    Vector<TypeParam> moved(std::move(original));

    EXPECT_EQ(moved.m_data, nullptr);
    EXPECT_EQ(moved.m_size, 0);
    EXPECT_EQ(moved.m_capacity, 0);

    EXPECT_EQ(original.m_data, nullptr);
    EXPECT_EQ(original.m_size, 0);
    EXPECT_EQ(original.m_capacity, 0);
}

TYPED_TEST(VectorTypedTests, DestructorTest) {
    { Vector<TypeParam> vec(5); }
}

TYPED_TEST(VectorTypedTests, CopyAssignmentTest) {
    const size_t count1 = 3;
    const size_t count2 = 5;
    Vector<TypeParam> vec1(count1);
    Vector<TypeParam> vec2(count2);

    vec1 = vec2;

    EXPECT_NE(vec1.m_data, vec2.m_data);
    EXPECT_EQ(vec1.m_size, vec2.m_size);
    EXPECT_EQ(vec1.m_capacity, vec2.m_capacity);

    for (size_t i = 0; i < count2; ++i) {
        EXPECT_EQ(vec1.m_data[i], vec2.m_data[i]);
    }
}

TYPED_TEST(VectorTypedTests, CopyAssignmentSelfTest) {
    const size_t count = 3;
    Vector<TypeParam> vec(count);
    auto original_data = vec.data();

    vec = vec;

    EXPECT_EQ(vec.data(), original_data);
    EXPECT_EQ(vec.size(), count);
    EXPECT_EQ(vec.capacity(), count);
}

TYPED_TEST(VectorTypedTests, CopyAssignmentEmptyToNonEmptyTest) {
    Vector<TypeParam> empty;
    Vector<TypeParam> nonEmpty(5);

    nonEmpty = empty;

    EXPECT_NE(nonEmpty.m_data, nullptr);
    EXPECT_EQ(nonEmpty.m_size, 0);
    EXPECT_EQ(nonEmpty.m_capacity, 0);
}

TYPED_TEST(VectorTypedTests, MoveAssignmentTest) {
    const size_t count1 = 3;
    const size_t count2 = 5;
    Vector<TypeParam> vec1(count1);
    Vector<TypeParam> vec2(count2);

    auto vec2_data = vec2.m_data;
    auto vec2_size = vec2.m_size;
    auto vec2_capacity = vec2.m_capacity;

    vec1 = std::move(vec2);

    EXPECT_EQ(vec1.m_data, vec2_data);
    EXPECT_EQ(vec1.m_size, vec2_size);
    EXPECT_EQ(vec1.m_capacity, vec2_capacity);

    EXPECT_EQ(vec2.m_data, nullptr);
    EXPECT_EQ(vec2.m_size, 0);
    EXPECT_EQ(vec2.m_capacity, 0);
}

TYPED_TEST(VectorTypedTests, MoveAssignmentSelfTest) {
    const size_t count = 3;
    Vector<TypeParam> vec(count);
    auto original_data = vec.data();

    vec = std::move(vec);

    EXPECT_EQ(vec.data(), original_data);
    EXPECT_EQ(vec.size(), count);
    EXPECT_EQ(vec.capacity(), count);
}

struct ThrowingType {
    int value;
    static int construction_count;
    static bool should_throw;

    ThrowingType() : value(0) {
        if (should_throw && ++construction_count > 3) {
            throw std::runtime_error("Construction failed");
        }
    }

    ThrowingType(int v) : value(v) {
        if (should_throw && ++construction_count > 3) {
            throw std::runtime_error("Construction failed");
        }
    }

    ThrowingType(const ThrowingType& other) : value(other.value) {
        if (should_throw && ++construction_count > 3) {
            throw std::runtime_error("Copy construction failed");
        }
    }

    ~ThrowingType() { --construction_count; }

    ThrowingType& operator=(const ThrowingType& other) {
        value = other.value;
        return *this;
    }
};

int ThrowingType::construction_count = 0;
bool ThrowingType::should_throw = false;

class VectorSpecificTests : public ::testing::Test {};

TEST_F(VectorSpecificTests, AllocatorTest) {
    std::allocator<int> alloc;
    Vector<int, std::allocator<int>> vec(5, alloc);

    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 5);
}

TEST_F(VectorSpecificTests, CustomAllocatorTest) {
    std::allocator<std::string> alloc;
    Vector<std::string, std::allocator<std::string>> vec(3, std::string("test"), alloc);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 3);
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(vec.data()[i], "test");
    }
}

TEST_F(VectorSpecificTests, LargeVectorTest) {
    const size_t large_size = 10000;
    Vector<int> vec(large_size);

    EXPECT_EQ(vec.size(), large_size);
    EXPECT_EQ(vec.capacity(), large_size);
    EXPECT_NE(vec.data(), nullptr);
}

TEST_F(VectorSpecificTests, ComplexTypeTest) {
    struct ComplexType {
        int value;
        std::string name;

        ComplexType() : value(0), name("default") {}
        ComplexType(int v, std::string n) : value(v), name(std::move(n)) {}

        bool operator==(const ComplexType& other) const { return value == other.value && name == other.name; }
    };

    Vector<ComplexType> vec(3);
    EXPECT_EQ(vec.size(), 3);

    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(vec.data()[i].value, 0);
        EXPECT_EQ(vec.data()[i].name, "default");
    }
}

TEST_F(VectorSpecificTests, IteratorBasicTest) {
    Vector<int> vec(5);

    Vector<int>::Iterator it(vec.data());
    Vector<int>::Iterator default_it;

    EXPECT_EQ(it.m_ptr, vec.data());
    EXPECT_EQ(default_it.m_ptr, nullptr);
}

TEST_F(VectorSpecificTests, IteratorDereferenceTest) {
    Vector<int> vec(1);
    vec.data()[0] = 42;

    Vector<int>::Iterator it(vec.data());
    EXPECT_EQ(*it, 42);
    EXPECT_EQ(it.operator->(), vec.data());
}

TEST_F(VectorSpecificTests, IteratorAssignmentTest) {
    Vector<int> vec(2);

    Vector<int>::Iterator it1(vec.data());
    Vector<int>::Iterator it2;

    it2 = it1;
    EXPECT_EQ(it2.m_ptr, it1.m_ptr);
}

TEST_F(VectorSpecificTests, ElementAccessAtTest) {
    Vector<int> vec(5);
    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i * 10);
    }

    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(vec.at(i), static_cast<int>(i * 10));
    }

    const Vector<int>& const_vec = vec;
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(const_vec.at(i), static_cast<int>(i * 10));
    }
}

TEST_F(VectorSpecificTests, ElementAccessAtOutOfBoundsTest) {
    Vector<int> vec(3);

    EXPECT_THROW(vec.at(3), std::out_of_range);
    EXPECT_THROW(vec.at(100), std::out_of_range);

    const Vector<int>& const_vec = vec;
    EXPECT_THROW(const_vec.at(3), std::out_of_range);
}

TEST_F(VectorSpecificTests, ElementAccessBracketOperatorTest) {
    Vector<int> vec(5);
    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i * 10);
    }

    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i * 10));
    }

    const Vector<int>& const_vec = vec;
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(const_vec[i], static_cast<int>(i * 10));
    }

    vec[0] = 999;
    EXPECT_EQ(vec[0], 999);
}

TEST_F(VectorSpecificTests, ElementAccessFrontTest) {
    Vector<int> vec(3);
    vec.data()[0] = 42;
    vec.data()[1] = 100;
    vec.data()[2] = 200;

    EXPECT_EQ(vec.front(), 42);

    const Vector<int>& const_vec = vec;
    EXPECT_EQ(const_vec.front(), 42);

    vec.front() = 999;
    EXPECT_EQ(vec.front(), 999);
}

TEST_F(VectorSpecificTests, ElementAccessBackTest) {
    Vector<int> vec(3);
    vec.data()[0] = 42;
    vec.data()[1] = 100;
    vec.data()[2] = 200;

    EXPECT_EQ(vec.back(), 200);

    const Vector<int>& const_vec = vec;
    EXPECT_EQ(const_vec.back(), 200);

    vec.back() = 999;
    EXPECT_EQ(vec.back(), 999);
}

TEST_F(VectorSpecificTests, ElementAccessDataTest) {
    Vector<int> vec(5);

    EXPECT_EQ(vec.data(), vec.data());

    const Vector<int>& const_vec = vec;
    EXPECT_EQ(const_vec.data(), vec.data());

    Vector<int> empty_vec;
    EXPECT_EQ(empty_vec.data(), nullptr);
}

TEST_F(VectorSpecificTests, IteratorBeginEndTest) {
    Vector<int> vec(5);
    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i);
    }

    auto it_begin = vec.begin();
    auto it_end = vec.end();

    EXPECT_EQ(it_begin.m_ptr, vec.data());
    EXPECT_EQ(it_end.m_ptr, vec.data() + 5);

    const Vector<int>& const_vec = vec;
    auto const_it_begin = const_vec.begin();
    auto const_it_end = const_vec.end();
    auto cbegin_it = const_vec.cbegin();
    auto cend_it = const_vec.cend();

    EXPECT_EQ(const_it_begin.m_ptr, vec.data());
    EXPECT_EQ(const_it_end.m_ptr, vec.data() + 5);
    EXPECT_EQ(cbegin_it.m_ptr, vec.data());
    EXPECT_EQ(cend_it.m_ptr, vec.data() + 5);
}

TEST_F(VectorSpecificTests, IteratorIncrementDecrementTest) {
    Vector<int> vec(5);
    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i * 10);
    }

    auto it = vec.begin();
    EXPECT_EQ(*it, 0);

    ++it;
    EXPECT_EQ(*it, 10);

    auto old_it = it++;
    EXPECT_EQ(*old_it, 10);
    EXPECT_EQ(*it, 20);

    --it;
    EXPECT_EQ(*it, 10);

    old_it = it--;
    EXPECT_EQ(*old_it, 10);
    EXPECT_EQ(*it, 0);
}

TEST_F(VectorSpecificTests, IteratorArithmeticTest) {
    Vector<int> vec(10);
    for (size_t i = 0; i < 10; ++i) {
        vec.data()[i] = static_cast<int>(i);
    }

    auto it = vec.begin();

    auto it_plus_3 = it + 3;
    EXPECT_EQ(*it_plus_3, 3);

    auto it_minus_2 = it_plus_3 - 2;
    EXPECT_EQ(*it_minus_2, 1);

    auto diff = it_plus_3 - it;
    EXPECT_EQ(diff, 3);
}

TEST_F(VectorSpecificTests, IteratorComparisonTest) {
    Vector<int> vec(5);

    auto it1 = vec.begin();
    auto it2 = vec.begin();
    auto it3 = vec.begin() + 2;

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);
    EXPECT_TRUE(it1 != it3);
    EXPECT_TRUE(it1 < it3);
    EXPECT_TRUE(it3 > it1);
    EXPECT_TRUE(it1 <= it2);
    EXPECT_TRUE(it1 <= it3);
    EXPECT_TRUE(it3 >= it1);
    EXPECT_TRUE(it2 >= it1);
}

TEST_F(VectorSpecificTests, CapacityEmptyTest) {
    Vector<int> empty_vec;
    EXPECT_TRUE(empty_vec.empty());

    Vector<int> non_empty_vec(5);
    EXPECT_FALSE(non_empty_vec.empty());
}

TEST_F(VectorSpecificTests, CapacitySizeTest) {
    Vector<int> empty_vec;
    EXPECT_EQ(empty_vec.size(), 0);

    Vector<int> vec_5(5);
    EXPECT_EQ(vec_5.size(), 5);

    Vector<int> vec_100(100);
    EXPECT_EQ(vec_100.size(), 100);
}

TEST_F(VectorSpecificTests, CapacityCapacityTest) {
    Vector<int> empty_vec;
    EXPECT_EQ(empty_vec.capacity(), 0);

    Vector<int> vec_5(5);
    EXPECT_EQ(vec_5.capacity(), 5);

    Vector<int> vec_100(100);
    EXPECT_EQ(vec_100.capacity(), 100);
}

TEST_F(VectorSpecificTests, CapacityMaxSizeTest) {
    Vector<int> vec;
    EXPECT_GT(vec.max_size(), 0);
    EXPECT_LE(vec.max_size(), std::numeric_limits<size_t>::max());
}

TEST_F(VectorSpecificTests, CapacityReserveTest) {
    Vector<int> vec(5);
    size_t original_capacity = vec.capacity();

    vec.reserve(10);
    EXPECT_GE(vec.capacity(), 10);
    EXPECT_EQ(vec.size(), 5);

    vec.reserve(5);
    EXPECT_GE(vec.capacity(), original_capacity);
}

TEST_F(VectorSpecificTests, CapacityShrinkToFitTest) {
    Vector<int> vec(10);
    vec.reserve(20);

    EXPECT_GE(vec.capacity(), 20);

    vec.shrink_to_fit();
    EXPECT_EQ(vec.capacity(), vec.size());
}

TEST_F(VectorSpecificTests, ModifiersClearTest) {
    Vector<int> vec(5);
    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i);
    }

    vec.clear();
    EXPECT_EQ(vec.size(), 0);
    EXPECT_GE(vec.capacity(), 0);
}

TEST_F(VectorSpecificTests, ModifiersInsertTest) {
    Vector<int> vec(3);
    vec.data()[0] = 1;
    vec.data()[1] = 3;
    vec.data()[2] = 5;

    auto it = vec.insert(vec.begin() + 1, 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 5);

    int value = 0;
    it = vec.insert(vec.begin(), std::move(value));
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0);
}

TEST_F(VectorSpecificTests, ModifiersEmplaceTest) {
    Vector<std::string> vec(2);
    vec.data()[0] = "hello";
    vec.data()[1] = "world";

    auto it = vec.emplace(vec.begin() + 1, "beautiful");
    EXPECT_EQ(*it, "beautiful");
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], "hello");
    EXPECT_EQ(vec[1], "beautiful");
    EXPECT_EQ(vec[2], "world");
}

TEST_F(VectorSpecificTests, ModifiersEraseTest) {
    Vector<int> vec(5);
    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i);
    }

    auto it = vec.erase(vec.begin() + 2);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);

    it = vec.erase(vec.begin() + 1, vec.begin() + 3);
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 4);
}

TEST_F(VectorSpecificTests, ModifiersPushBackTest) {
    Vector<int> vec;

    vec.push_back(42);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 42);

    int value = 100;
    vec.push_back(std::move(value));
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], 100);
}

TEST_F(VectorSpecificTests, ModifiersEmplaceBackTest) {
    Vector<std::string> vec;

    auto& ref = vec.emplace_back("hello", 5);
    EXPECT_EQ(ref, "hello");
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], "hello");

    vec.emplace_back("world");
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], "world");
}

TEST_F(VectorSpecificTests, ModifiersPopBackTest) {
    Vector<int> vec(3);
    vec.data()[0] = 1;
    vec.data()[1] = 2;
    vec.data()[2] = 3;

    vec.pop_back();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);

    vec.pop_back();
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 1);
}

TEST_F(VectorSpecificTests, ModifiersResizeTest) {
    Vector<int> vec(3);
    for (size_t i = 0; i < 3; ++i) {
        vec.data()[i] = static_cast<int>(i);
    }

    vec.resize(5);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], int{});
    EXPECT_EQ(vec[4], int{});

    vec.resize(2);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);

    vec.resize(4, 99);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 99);
    EXPECT_EQ(vec[3], 99);
}

TEST_F(VectorSpecificTests, ModifiersSwapTest) {
    Vector<int> vec1(3);
    Vector<int> vec2(5);

    for (size_t i = 0; i < 3; ++i) {
        vec1.m_data[i] = static_cast<int>(i);
    }
    for (size_t i = 0; i < 5; ++i) {
        vec2.m_data[i] = static_cast<int>(i * 10);
    }

    auto vec1_data = vec1.m_data;
    auto vec2_data = vec2.m_data;

    vec1.swap(vec2);

    EXPECT_EQ(vec1.m_data, vec2_data);
    EXPECT_EQ(vec2.m_data, vec1_data);
    EXPECT_EQ(vec1.size(), 5);
    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec1.capacity(), 5);
    EXPECT_EQ(vec2.capacity(), 3);
}

TEST_F(VectorSpecificTests, AllocatorGetAllocatorTest) {
    Vector<int> vec;
    auto alloc = vec.get_allocator();
    static_cast<void>(alloc);
}

TEST_F(VectorSpecificTests, AssignInitializerListTest) {
    Vector<int> vec(5);

    vec = {1, 2, 3};
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(VectorSpecificTests, AssignCountValueTest) {
    Vector<int> vec(5);

    vec.assign(3, 42);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 42);
    EXPECT_EQ(vec[1], 42);
    EXPECT_EQ(vec[2], 42);
}

TEST_F(VectorSpecificTests, AssignInitializerListTest2) {
    Vector<int> vec(5);

    vec.assign({10, 20, 30, 40});
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
    EXPECT_EQ(vec[3], 40);
}

TEST_F(VectorSpecificTests, ExceptionSafetyTest) {
    ThrowingType::construction_count = 0;
    ThrowingType::should_throw = true;

    EXPECT_THROW(Vector<ThrowingType> vec(10), std::runtime_error);
    EXPECT_EQ(ThrowingType::construction_count, 0);

    ThrowingType::should_throw = false;
}

TEST_F(VectorSpecificTests, EdgeCaseEmptyVectorTest) {
    Vector<int> vec;

    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.data(), nullptr);
}

TEST_F(VectorSpecificTests, EdgeCaseSingleElementTest) {
    Vector<int> vec(1);
    vec.data()[0] = 42;

    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(vec.front(), 42);
    EXPECT_EQ(vec.back(), 42);
    EXPECT_EQ(vec[0], 42);
    EXPECT_EQ(vec.at(0), 42);
}

TEST_F(VectorSpecificTests, MemoryLayoutTest) {
    Vector<int> vec(5);

    for (size_t i = 0; i < 5; ++i) {
        vec.data()[i] = static_cast<int>(i);
    }

    for (size_t i = 1; i < 5; ++i) {
        EXPECT_EQ(&vec.data()[i], &vec.data()[i - 1] + 1);
    }
}