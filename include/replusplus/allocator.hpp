#include <new>

template <typename T>
struct allocator {
    using value_type = T;

    allocator() noexcept = default;

    // Containers that store your object plus other data (e.g., nodes in std::list) may need to convert allocator<T> to allocator<Node>
    template <typename U>
    allocator(const allocator<U>&) noexcept {}

    // Allocate raw memory without constructing n objects of type T
    [[nodiscard]] T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    // Just deallocate, do not destroy
    void deallocate(T* p, std::size_t n) noexcept {
        ::operator delete(p);
    }
};