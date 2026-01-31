#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <new>

/*
    Dynamically resizable vector with a few basic things
*/

constexpr size_t GROWTH_FACTOR = 2ull;

namespace fun {

template <typename T> class vector {
public:
    vector() : data_{nullptr}, size_{0ULL}, capacity_{0ULL} {}
    vector(const std::initializer_list<T> &list)
        : data_{}, size_{list.size()}, capacity_{list.size()} {
        data_ = raw_alloc_arr(size_);
        const T* list_arr = list.begin();
        for (size_t i = 0; i < size_; i++) {
            new (data_ + i) T(list_arr[i]);
        }
    }
    ~vector() {
        destroy_arr_elements(data_, size_);
        delete_arr(data_);
    }

    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    T &operator[](size_t idx) { return data_[idx]; }
    T &at(size_t idx) {
        if (idx > size_) {
            throw std::out_of_range("Value accessed out of range!");
        }

        return data_[idx];
    }

private:
    T *data_;
    size_t size_;
    size_t capacity_;

    T *raw_alloc_arr(size_t num_elements) {
        T *arr = static_cast<T *>(::operator new[](num_elements * sizeof(T)));
        return arr;
    }
    void initialize_arr(T *arr, size_t num_elements, const T& val) {
        for (size_t i = 0; i < num_elements; i++) {
            new (arr + i) T(val);
        }
    }
    void destroy_arr_elements(T *arr, size_t num_elements) {
        for (size_t i = 0; i < num_elements; i++) {
            arr[i].~T();
        }
    }
    void delete_arr(T *arr) {
        ::operator delete[](arr);
        arr = nullptr;
    }
};

template <typename T> vector(std::initializer_list<T>) -> vector<T>;

}; // namespace fun
