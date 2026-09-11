#include <replusplus/allocator.hpp>
#include <gtest/gtest.h>
#include <memory>

struct Tracker {
    int value_;

    // static => one shared by all Tracker instances
    // inline => we want to define AND initialize inside the class as opposed to Tracker::constructions = 0 outside
    static inline int constructions = 0;
    static inline int destructions = 0;

    explicit Tracker(int value) : value_{value} {
        constructions++;
    }

    ~Tracker() {
        destructions++;
    }
};

TEST(AllocatorTest, AllocateAndDeallocate) {
    using Alloc = replusplus::allocator<int>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc;

    int* p = Traits::allocate(alloc, 4);

    ASSERT_NE(p, nullptr);

    Traits::deallocate(alloc, p, 4);
}

TEST(AllocatorTest, AllocateDoesNotConstruct) {
    using Alloc = replusplus::allocator<Tracker>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc;

    Tracker* p = Traits::allocate(alloc, 67);

    ASSERT_NE(p, nullptr);
    EXPECT_EQ(Tracker::constructions, 0);

    Traits::deallocate(alloc, p, 67);
}

TEST(AllocatorTest, DeallocateDoesNotDestroy) {
    using Alloc = replusplus::allocator<Tracker>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc;

    const int constructions_before = Tracker::constructions;
    const int destructions_before = Tracker::destructions;

    Tracker* p = Traits::allocate(alloc, 1);

    Traits::construct(alloc, p, 67);

    EXPECT_EQ(Tracker::constructions, constructions_before + 1);

    Traits::deallocate(alloc, p, 1);

    EXPECT_EQ(Tracker::destructions, destructions_before);
}

TEST(AllocatorTest, ConstructAndDestroy) {
    using Alloc = replusplus::allocator<Tracker>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc;

    const int constructions_before = Tracker::constructions;
    const int destructions_before = Tracker::destructions;

    Tracker* p = Traits::allocate(alloc, 1);

    Traits::construct(alloc, p, 42);

    EXPECT_EQ(Tracker::constructions, constructions_before + 1);
    EXPECT_EQ(p->value_, 42);

    Traits::destroy(alloc, p);

    EXPECT_EQ(Tracker::destructions, destructions_before + 1);

    Traits::deallocate(alloc, p, 1);
}