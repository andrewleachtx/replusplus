#include <cstddef>
#include <mutex>

namespace replusplus {

template <typename T> struct control_block {
    std::size_t reference_count_{0};
    std::mutex mutex_;
};

template <typename T> class shared_ptr {
public:
    shared_ptr() {}
    shared_ptr(T* pointer) {}
    ~shared_ptr() {}

    shared_ptr& operator=(const shared_ptr& other);
    shared_ptr(const shared_ptr& other);

    shared_ptr& operator=(shared_ptr&&) = delete;
    shared_ptr(shared_ptr&&) = delete;

    T* get() { return data_; }
    T& operator*() const noexcept { return *data_; }
    T* operator->() const noexcept { return data_; }

private:
    control_block<T>* ctrl_blk_;
    T* data_;
};

} // namespace replusplus