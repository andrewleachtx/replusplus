/*
    Spec for new vector

    - should use a custom allocator and placement new
        - allocator_traits<Allocator>
    - construct in place, not on allocation
    - implement its iterator
    - constexpr support
    - ctors
        - default, vector(count), vector(count, value)
        -
    - apis
        - insert, erase, push_back, move push_back, pop_back, pop_front
        - reserve(new_capacity), shrink_to_fit(), clear(),
        - access[], .at(), .front(), .back(), .data()
        - 
*/

template <typename T, typename Allocator=std::allocator<T>>
class vector {};