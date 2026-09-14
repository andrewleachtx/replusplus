#include <initializer_list>
#include <stdexcept>
#include <string>

#include <cassert>
#include <memory>
#include <replusplus/allocator.hpp>
#include <utility>

#pragma once

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
namespace replusplus {
template <typename T, typename Allocator = replusplus::allocator<T>>
class vector {
public:
    using allocator_traits = std::allocator_traits<Allocator>;
    using value_type = T;
    using size_type = std::size_t;

    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return (size_ == 0) ? data_ : data_ + size_; }
    const_iterator end() const noexcept {
        return (size_ == 0) ? data_ : data_ + size_;
    }
    const_iterator cend() const noexcept {
        return (size_ == 0) ? data_ : data_ + size_;
    }

    // Constructors
    vector() : data_{nullptr}, size_{0}, capacity_{0} {}
    vector(std::initializer_list<T> ilist)
        : data_{nullptr}, size_{0}, capacity_{0} {
        if (ilist.size() == 0) {
            return;
        }

        data_ = allocator_traits::allocate(alloc_, ilist.size());
        capacity_ = ilist.size();

        for (const auto& v : ilist) {
            allocator_traits::construct(alloc_, data_ + size_, v);
            size_++;
        }
    }

    vector(const vector& other)
        : data_{nullptr}, size_{0}, capacity_{0},
          alloc_{allocator_traits::select_on_container_copy_construction(
              other.alloc_)} {
        copyFrom(other);
    }
    vector(vector&& other)
        : data_{nullptr}, size_{0}, capacity_{0},
          alloc_{std::move(other.alloc_)} {
        moveFrom(std::move(other));
    }

    vector& operator=(const vector& other) {
        if (this != &other) {
            copyFrom(other);
        }

        return *this;
    }
    vector& operator=(vector&& other) {
        if (this != &other) {
            moveFrom(std::move(other));
        }

        return *this;
    }

    ~vector() {
        clear();

        if (data_) {
            allocator_traits::deallocate(alloc_, data_, capacity_);
        }
    }

    // Access
    T& at(const std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range(
                "replusplus::vector::at(index=" + std::to_string(index) +
                ") access is out of bounds on vector with size=" +
                std::to_string(size_));
        }

        return data_[index];
    }
    const T& at(const std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range(
                "replusplus::vector::at(index=" + std::to_string(index) +
                ") access is out of bounds on vector with size=" +
                std::to_string(size_));
        }

        return data_[index];
    }

    T& operator[](const std::size_t index) { return data_[index]; }
    const T& operator[](const std::size_t index) const { return data_[index]; }

    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }

    T& back() { return data_[size_ - 1]; }
    const T& back() const { return data_[size_ - 1]; }

    T* data() { return data_; }
    const T* data() const { return data_; }

    // Capacity
    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    void reserve(std::size_t desired_capacity);
    void shrink_to_fit();

    // Modifiers
    void clear() {
        for (std::size_t i = 0; i < size_; i++) {
            allocator_traits::destroy(alloc_, data_ + i);
        }

        size_ = 0;
    }
    iterator insert(const_iterator pos, const T& value) {
        // nullptr - nullptr is UB
        std::size_t index = (pos == begin()) ? 0 : (pos - begin());

        // If incrementing once would exceed our capacity, we must resize which can invalidate our pos
        if (size_ == capacity_) {
            reallocateTo((capacity_ == 0) ? 1 : capacity_ * GROWTH_FACTOR);
        }

        // If we try to insert at begin() and the vector is empty, we don't want to cause underflow with size_ - 1 = 0 - 1, so handle that case separately
        if (index == size_) {
            allocator_traits::construct(alloc_, data_ + size_, value);
        }
        else {
            // Construct at the last slot so later shifts can place there
            allocator_traits::construct(
                alloc_, data_ + size_, std::move_if_noexcept(data_[size_ - 1]));

            // Shift all elements starting at pos over to the right by one (work right to left)
            for (std::size_t i = size_ - 1; i > index; i--) {
                // Move i-1 into i
                data_[i] = std::move(data_[i - 1]);
            }

            data_[index] = value;
        }

        size_++;
        return data_ + index;
    }
    iterator insert(const_iterator pos, T&& value) {
        // nullptr - nullptr is UB
        std::size_t index = (pos == cbegin()) ? 0 : (pos - cbegin());

        // If incrementing once would exceed our capacity, we must resize which can invalidate our pos
        if (size_ == capacity_) {
            reallocateTo((capacity_ == 0) ? 1 : capacity_ * GROWTH_FACTOR);
        }

        // If we try to insert at end() or the vector is empty we don't need to shift
        if (index == size_) {
            allocator_traits::construct(alloc_, data_ + size_,
                                        std::move(value));
        }
        else {
            // Construct at the last slot so later shifts can place there
            allocator_traits::construct(
                alloc_, data_ + size_, std::move_if_noexcept(data_[size_ - 1]));

            // Shift all elements starting at pos over to the right by one (work right to left)
            for (std::size_t i = size_ - 1; i > index; i--) {
                // Move i-1 into i
                data_[i] = std::move(data_[i - 1]);
            }

            data_[index] = std::move(value);
        }

        size_++;
        return data_ + index;
    }

    iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
    iterator erase(const_iterator first, const_iterator last) {
        // Erasing an empty range is a no-op
        if (first == last) {
            if (first == cbegin()) {
                return begin();
            }

            const std::size_t idx = static_cast<std::size_t>(first - cbegin());

            return data_ + idx;
        }

        std::size_t idx = static_cast<std::size_t>(first - cbegin());
        const std::size_t erase_ct = static_cast<std::size_t>(last - first);

        // Shift erase_ct elements over
        for (std::size_t i = idx; i + erase_ct < size_; i++) {
            data_[i] = std::move(data_[i + erase_ct]);
        }

        for (std::size_t i = size_ - erase_ct; i < size_; i++) {
            // Destroy where we moved from
            allocator_traits::destroy(alloc_, data_ + i);
        }

        size_ -= erase_ct;
        return data_ + idx;
    }

    void push_back(const T& value) { insert(cend(), value); }
    void push_back(T&& value) { insert(cend(), std::move(value)); }

    void pop_back() {
        assert(!empty() && "pop_back called on an empty vector!");
        erase(cend());
    }
    void pop_front() {
        assert(!empty() && "pop_front called on an empty vector!");
        erase(cbegin());
    }
    void pop(std::size_t index) {
        assert(index < size_ && "pop called with an out-of-bounds index!");
        erase(cbegin() + index);
    }

    void resize(std::size_t desired_count) {}
    void swap(vector& other) {}

    // operator<=>

private:
    static constexpr std::size_t GROWTH_FACTOR = 2;

    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    [[no_unique_address]] Allocator alloc_{};

    // If this private helper is called, we will attempt to reallocate to the desired capacity regardless of checks (e.g., even if desired_capacity < cur_size)
    void reallocateTo(std::size_t desired_capacity) {
        assert(desired_capacity >= size_ &&
               "reallocateTo requested with a desired capacity < size!");

        T* new_data = nullptr;

        if (desired_capacity > 0) {
            new_data = allocator_traits::allocate(alloc_, desired_capacity);
        }

        for (std::size_t i = 0; i < size_; i++) {
            allocator_traits::construct(alloc_, new_data + i,
                                        std::move_if_noexcept(data_[i]));
        }

        for (std::size_t i = 0; i < size_; i++) {
            allocator_traits::destroy(alloc_, data_ + i);
        }

        if (data_) {
            allocator_traits::deallocate(alloc_, data_, capacity_);
        }

        data_ = new_data;
        capacity_ = desired_capacity;
    }

    // todo, make exception safe with construction count and rollback
    void copyFrom(const vector& other) {
        clear();

        if (data_) {
            allocator_traits::deallocate(alloc_, data_, capacity_);
        }

        // Hollow out in case of exception later
        data_ = nullptr;
        capacity_ = 0;

        if (other.capacity_ == 0) {
            return;
        }

        data_ = allocator_traits::allocate(alloc_, other.capacity_);
        capacity_ = other.capacity_;

        for (std::size_t i = 0; i < other.size_; i++) {
            allocator_traits::construct(alloc_, data_ + i, other.data_[i]);
        }

        size_ = other.size_;
    }
    void moveFrom(vector&& other) {
        clear();

        if (data_) {
            allocator_traits::deallocate(alloc_, data_, capacity_);
        }

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
};
}; // namespace replusplus
