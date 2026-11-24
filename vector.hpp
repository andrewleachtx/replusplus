#pragma once

#include <cstddef>
#include <stdexcept>

namespace rpp {
    
template <typename T>
class vector {
public:
    vector(const vector& other) {
        copyFrom_(other);
    }
    vector& operator=(const vector& other) {
        if (this != &other) {
            copyFrom_(other);
        }   
        
        return *this;
    }
    vector(vector&& other) {
        moveFrom_(other);
    }
    vector& operator=(vector&& other) {
        if (this != &other) {
            moveFrom_(other);
        }   
        
        return *this;
    }
    
    // insert value at index such that vector[index] = value afterwards
    void insert(const T& value, std::size_t index) {
        // if our size is equal to capacity, we cannot perform this insertion, and must double
        if (size() == capacity()) {
            grow_();
        }

        // shift everything after to the right
        for (T* it = last_; it > first_ + index; it--) {
            *(it) = *(it - 1);
        }
        first_[index] = value;
        last_++;
    }

    T& operator[](std::size_t index) {
        return *(first_ + index);
    }
    T& at(std::size_t index) {
        if (first_ + index > last_ || first_ + index < first_) {
            throw std::out_of_range(".at(...) called with an invalid index"); 
        }
        return *(first_ + index);
    }
    std::size_t size() const {
        return last_ - first_;
    }
    std::size_t capacity() const {
        return end_ - first_;
    }
private:
    T* first_;
    T* last_;
    T* end_;

    void grow_() {

    }
    void copyFrom_() {}
    void moveFrom_() {}
}; 

} // namespace rpp