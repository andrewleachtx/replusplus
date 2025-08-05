#include <memory>

const size_t GROWTH_FACTOR = 2UL;

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator;
    using const_iterator = const Iterator;
    
    struct Iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Iterator() = default;
            explicit Iterator(pointer ptr) : m_ptr(ptr) {}
            Iterator &operator=(const Iterator&) noexcept = default;

            [[nodiscard]] reference operator*() const {
                return *ptr;
            }
            [[nodiscard]] pointer operator->() const {
                return ptr;
            }

            Iterator& operator++() const { /* TODO */ }
            Iterator operator++(int) { /* TODO */ }
            Iterator& operator--() const { /* TODO */ }
            Iterator operator--(int) { /* TODO */ }

            [[nodiscard]] Iterator operator+(difference_type n) const { /* TODO */ }
            [[nodiscard]] Iterator operator-(difference_type n) const { /* TODO */ }
            difference_type operator-(const Iterator &rhs) { /* TODO */ }
            
            auto operator<=>(const Iterator &rhs) const = default; // TODO If you want
        
        private:
            pointer m_ptr = nullptr;
    };

    Vector() { /* TODO */ }
    explicit Vector(size_type count, const allocator_type &alloc = allocator_type()) { /* TODO */ }
    constexpr Vector(size_type count, const_reference value = value_type(), const allocator_type &alloc = allocator_type()) { /* TODO */ }
    constexpr Vector(const Vector &other) { /* TODO */ }
    constexpr Vector(Vector &&other) noexcept { /* TODO */ }
    constexpr Vector& operator=(const Vector &other) { /* TODO */ }

    Vector& operator=(Vector &&other) noexcept { /* TODO */ }
    Vector& operator=(std::initializer_list<value_type> ilist) { /* TODO */ }
    
    constexpr ~Vector() { /* TODO */ }

    constexpr void assign(size_type count, const_reference value) { /* TODO */ }
    constexpr void assign(std::initializer_list<value_type> ilist) { /* TODO */ }
    
    constexpr allocator_type get_allocator const { /* TODO */ }

    constexpr reference at(size_type pos) { /* TODO */ }
    constexpr const_reference at(size_type pos) { /* TODO */ }

    constexpr reference operator[](size_type pos) { /* TODO */ }
    constexpr const_reference operator[](size_type pos) const { /* TODO */ }

    constexpr reference front() { /* TODO */ }
    constexpr const_reference const front() { /* TODO */ }

    constexpr reference back() { /* TODO */ }
    constexpr const_reference back() const { /* TODO */ }

    constexpr pointer data() noexcept { /* TODO */ }
    constexpr const_pointer data() const noexcept const { /* TODO */ }

    constexpr iterator begin() noexcept { /* TODO */ }
    constexpr const_iterator begin() const noexcept { /* TODO */ }
    constexpr const_iterator cbegin() const noexcept { /* TODO */ }
    
    constexpr iterator end() noexcept { /* TODO */ }
    constexpr const_iterator end() const noexcept { /* TODO */ }
    constexpr const_iterator cend() const noexcept { /* TODO */ }

    private:
        pointer m_arr;
        [[no_unique_address]] allocator_type m_alloc;
        size_type m_size;
        size_type m_capacity;
};