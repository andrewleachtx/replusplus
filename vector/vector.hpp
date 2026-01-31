#include <iostream>

/*
    Dynamically resizable vector with a few basic things
*/

namespace fun {

template <typename T>
class vector {
public:
    vector() : data_{}, size_{}, capacity_{} {}
private:
    T* data_;
    size_t size_;
    size_t capacity_;
};
}; // namespace fun
