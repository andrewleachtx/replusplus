// TODO v
// #include <replusplus/deque.hpp>
#include <cstddef>
#include <deque>

namespace replusplus {
template <typename T>
// todo can we add a concept here for is container type? what apis do we require
class queue {
    queue() {}
    ~queue() {}

    queue& operator=(const queue& other) {}
    queue& operator=(queue&& other) {}
    queue(const queue& other) {}
    queue(queue&& other) {}

    std::size_t size() const noexcept { return data_.size(); }
    bool empty() const noexcept { return data_.empty(); }

    void push() {}
    void pop() {}

    // Return the guy in the front of the line
    T& front() { return data_.front(); }
    T& back() { return data_.back(); }

private:
    // replusplus::deque<T> data_;
    std::deque<T> data_;
};

} // namespace replusplus