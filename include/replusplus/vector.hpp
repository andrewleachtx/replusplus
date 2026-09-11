#include <memory>
#include <string>

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
*/

template <typename T, typename Allocator=std::allocator<T>>
class vector {
public:
    using allocator_traits = std::allocator_traits<Allocator>;
    using value_type = T;
    using size_type = std::size_t;

    vector() {}
    ~vector() {}

    // Access
    T& at(const std::size_t index) {
        if (index >= size_) {
            // throw std::out_of_range(std::format(".at(index={}) access out of bounds on vector with size={}", index, size_));
            throw std::out_of_range(
                "replusplus::vector::at(index=" + std::to_string(index) + ") access is out of bounds on vector with size=" + std::to_string(size_)
            );
        }

        return data_[index];
    }
    T& operator[](const std::size_t index) {}
    T& front(const std::size_t index) {}
    T& back(const std::size_t index) {}
    T* data() { return data_; }

    // Capacity
    bool empty() const noexcept { return size_ == 0; } 
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    void reserve(std::size_t desired_capacity);
    void shrink_to_fit();

    // Modifiers
    void clear() {}
    // TODO iterator insert https://en.cppreference.com/cpp/container/vector/insert
    // TODO iterator erase https://en.cppreference.com/cpp/container/vector/erase
    void push_back(const T& value) {}
    void push_back(T&& value) {}
    void pop_back() {}
    void resize(std::size_t desired_count) {}
    void swap(vector& other) {}

    // operator<=>

private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    Allocator allocator_;
};