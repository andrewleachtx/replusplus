#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <new>
#include <utility>

/*
    Dynamically resizable vector with a few basic things
*/

constexpr size_t GROWTH_FACTOR = 2;

namespace fun {

template <typename T> class vector {
public:
    vector() : data_{}, size_{0}, capacity_{1} {
        data_ = raw_alloc_arr(capacity_);
    }
    vector(const std::initializer_list<T>& list)
        : data_{}, size_{list.size()}, capacity_{list.size()} {
        data_ = raw_alloc_arr(size_);
        const T* list_arr = list.begin();
        copy_into_arr(data_, list_arr, size_);
    }
    vector& operator=(const vector& other) {
        if (this != &other) {
            // Deallocate the elements we have
            clear();

            // Only do a realloc if the other buffer has a bigger capacity
            if (other.capacity_ > capacity_) {
                delete_arr(data_);
                data_ = raw_alloc_arr(other.capacity_);

                capacity_ = other.capacity_;
            }

            copy_into_arr(data_, other.data_, other.size_);
            size_ = other.size_;
        }

        return *this;
    }
    vector(const vector& other)
        : data_{}, size_{other.size_}, capacity_{other.capacity_} {
        data_ = raw_alloc_arr(other.capacity_);
        copy_into_arr(data_, other.data_, other.size_);
    }
    vector& operator=(vector&& other) {
        if (this != &other) {
            // Deallocate the elements we have
            clear();
            delete_arr(data_);

            data_ = other.data_;
            capacity_ = other.capacity_;
            size_ = other.size_;
            
            other.data_ = nullptr;
            other.capacity_ = 0;
            other.size_ = 0;
        }

        return *this;
    }
    vector(vector&& other)
        : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_} {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    ~vector() {
        clear();
        delete_arr(data_);
    }

    // TODO(A): can we make this strong exception safe (support rollback)?
    void push_back(const T& val) {
        // Try placing value at size_ (data_[size_] = val), resize if needed
        if (size_ == capacity_) {
            reserve(capacity_ * GROWTH_FACTOR);
        }

        new (data_ + size_) T(val);
        size_++;
    }
    void push_back(T&& val) {
        // Try placing value at size_ (data_[size_] = val), resize if needed
        if (size_ == capacity_) {
            reserve(capacity_ * GROWTH_FACTOR);
        }

        new (data_ + size_) T(std::move(val));
        size_++;
    }
    void clear() {
        destroy_arr_elements(data_, size_);
        size_ = 0;
    }
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            // Alloc new capacity and std::move existing elements into
            T* new_data = raw_alloc_arr(new_capacity);
            move_into_arr(new_data, data_, size_);

            // Deallocate old data_
            destroy_arr_elements(data_, size_);
            delete_arr(data_);
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }
    void resize(size_t count) {}
    void resize(size_t count, const T& val) {}

    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    T& operator[](size_t idx) { return data_[idx]; }
    T& at(size_t idx) {
        if (idx > size_) {
            throw std::out_of_range("Value accessed out of range!");
        }

        return data_[idx];
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;

    T* raw_alloc_arr(size_t desired_capacity) {
        T* arr =
            static_cast<T*>(::operator new[](desired_capacity * sizeof(T)));
        return arr;
    }
    void init_arr(T* arr, size_t count, const T& val) {
        for (size_t i = 0; i < count; i++) {
            new (arr + i) T(val);
        }
    }
    void copy_into_arr(T* dst, const T* src, size_t count) {
        for (size_t i = 0; i < count; i++) {
            new (dst + i) T(src[i]);
        }
    }
    void move_into_arr(T* dst, T* src, size_t count) {
        for (size_t i = 0; i < count; i++) {
            new (dst + i) T(std::move(src[i]));
        }
    }
    void destroy_arr_elements(T* arr, size_t count) {
        for (size_t i = 0; i < count; i++) {
            arr[i].~T();
        }
    }
    void delete_arr(T* arr) {
        ::operator delete[](arr);
        arr = nullptr;
    }
};

template <typename T> vector(std::initializer_list<T>) -> vector<T>;

}; // namespace fun
