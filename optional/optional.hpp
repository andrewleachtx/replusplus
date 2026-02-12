#include <stdexcept>
#include <utility>

/*
    fun::optional<Dog> dog;
        -> has_value_ should be false
        -> the obj_ technically will be initialized, which is bad but 
           room for improvement
*/
namespace fun {
template <typename T> class optional {
public:
    optional() : obj_{}, has_value_{false} {}
    optional(T obj) : obj_{obj}, has_value_{true} {}
    optional(const optional& other)
        : obj_{other.obj_}, has_value_{other.has_value_} {}
    optional& operator=(const optional& other) {
        if (this != &other) {
            obj_ = other.obj_;
            has_value_ = other.has_value_;
        }

        return *this;
    }
    optional(optional&& other)
        : obj_{std::move(other.obj_)}, has_value_{other.has_value_} {
        other.has_value_ = false;
    }
    optional& operator=(optional&& other) {
        if (this != &other) {
            obj_ = std::move(other.obj_);
            has_value_ = other.has_value_;

            other.has_value_ = false;
        }

        return *this;
    }
    ~optional() {}

    T& value() {
        if (!has_value_) {
            throw std::logic_error("value() requested in empty optional!");
        }
        return obj_;
    }
    bool has_value() const noexcept { return has_value_; }

    T& operator*() noexcept { return obj_; }
    T* operator->() noexcept { return &obj_; }
    operator bool() const noexcept { return has_value(); }

private:
    T obj_;
    bool has_value_;
};
}; // namespace fun
