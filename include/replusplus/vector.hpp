#include <stdexcept>
#include <string>

#include <memory>
#include <replusplus/allocator.hpp>

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

    // Constructors
    vector() : data_{nullptr}, size_{0}, capacity_{0} {}

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
    // TODO iterator insert https://en.cppreference.com/cpp/container/vector/insert
    // TODO iterator erase https://en.cppreference.com/cpp/container/vector/erase
    void push_back(const T& value) {}
    void push_back(T&& value) {}
    void pop_back() {}
    void pop_front() {}
    void pop(std::size_t index) {}

    void resize(std::size_t desired_count) {}
    void swap(vector& other) {}

    // operator<=>

private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    [[no_unique_address]] Allocator alloc_{};

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
