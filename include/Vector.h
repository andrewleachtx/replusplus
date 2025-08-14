#include <memory>

const size_t GROWTH_FACTOR = 2UL;

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    struct Iterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() = default;
        explicit Iterator(pointer ptr) : m_ptr(ptr) {}
        Iterator &operator=(const Iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const {
            return *m_ptr;
        }
        [[nodiscard]] pointer operator->() const {
            return m_ptr;
        }

        Iterator& operator++() const { /* TODO */ }
        Iterator operator++(int) { /* TODO */ }
        Iterator& operator--() const { /* TODO */ }
        Iterator operator--(int) { /* TODO */ }

        [[nodiscard]] Iterator operator+(difference_type n) const { /* TODO */ }
        [[nodiscard]] Iterator operator-(difference_type n) const { /* TODO */ }
        difference_type operator-(const Iterator &rhs) { /* TODO */ }
        
        auto operator<=>(const Iterator &rhs) const = default; // TODO If you want

        pointer m_ptr = nullptr;
    };

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
    
    using traits = std::allocator_traits<allocator_type>;
    
    Vector() = default;
    explicit Vector(size_type count, const allocator_type &alloc = allocator_type()) : m_data(nullptr) , m_size(count), m_capacity(count), m_alloc(alloc) {
        m_data = traits::allocate(alloc, count);
        
        for (size_type i = 0; i < count; i++) {
            std::allocator<allocator_type>::construct(m_alloc, m_data + i, value_type());
        }
    }
    constexpr Vector(size_type count, const_reference value = value_type(), const allocator_type &alloc = allocator_type()) {
        m_data = traits::allocate(m_alloc, count);
        
        for (size_type i = 0; i < count; i++) {
            std::allocator<allocator_type>::construct(m_alloc, m_data + i, value);
        }
    }
    constexpr Vector(const Vector &other)
    : m_data(nullptr),
    m_size(other.m_size),
    m_capacity(other.m_capacity),
    m_alloc(traits::select_on_container_copy_construction(other.m_alloc)) {
        
        m_data = traits::allocate(m_alloc, m_capacity);
        
        for (size_type i = 0; i < m_size; i++) {
            traits::construct(m_alloc, m_data + i, other.m_data[i]);
        }
    }
    constexpr Vector(Vector &&other) noexcept
    : m_data(other.m_data),
    m_size(other.m_size),
    m_capacity(other.m_capacity),
    m_alloc(std::move(other.m_alloc)) {
        
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_alloc = allocator_type {};
    }
    
    ~Vector() {
        for (size_type i = 0; i < m_size; i++) {
            traits::destroy(m_alloc, m_data + i);
        }
        
        if (m_data) {
            traits::deallocate(m_alloc, m_data, m_capacity);
        }
    }
    
    constexpr Vector& operator=(const Vector &other) {
        if (this != &other) {
            // Assume we need to destroy all of our values even if we don't
            for (size_type i = 0; i < m_size; i++) {
                traits::destroy(m_alloc, m_data + i);
            }
            
            // TODO: Possibly check for difference in capacity to avoid unnecessary alloc/dealloc
            traits::deallocate(m_alloc, m_data, m_capacity);
            
            if constexpr (traits::propagate_on_container_copy_assignment::value) {
                
            }
            m_alloc = traits::select_on_container_copy_construction(other.m_alloc);
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            
            m_data = traits::allocate(m_alloc, m_capacity);            
            for (size_type i = 0; i < m_size; i++) {
                traits::construct(m_alloc, m_data + i, other.m_data[i]);
            }
        }
        
        return *this;
    }
    Vector& operator=(Vector &&other) noexcept {
        if (this != &other) {
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_alloc = std::move(other.m_alloc);
            
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_alloc = allocator_type {};
        }
        
        return *this;
    }
    Vector& operator=(std::initializer_list<value_type> ilist) { /* TODO */ }

    constexpr void assign(size_type count, const_reference value) { /* TODO */ }
    constexpr void assign(std::initializer_list<value_type> ilist) { /* TODO */ }
    
    constexpr allocator_type get_allocator() const { /* TODO */ }

    constexpr reference at(size_type pos) { /* TODO */ }
    constexpr const_reference at(size_type pos) const { /* TODO */ }

    constexpr reference operator[](size_type pos) { /* TODO */ }
    constexpr const_reference operator[](size_type pos) const { /* TODO */ }

    constexpr reference front() { /* TODO */ }
    constexpr const_reference front() const { /* TODO */ }

    constexpr reference back() { /* TODO */ }
    constexpr const_reference back() const { /* TODO */ }

    constexpr pointer data() noexcept { /* TODO */ }
    constexpr const_pointer data() const noexcept { /* TODO */ }

    constexpr iterator begin() noexcept { /* TODO */ }
    constexpr const_iterator begin() const noexcept { /* TODO */ }
    constexpr const_iterator cbegin() const noexcept { /* TODO */ }
    
    constexpr iterator end() noexcept { /* TODO */ }
    constexpr const_iterator end() const noexcept { /* TODO */ }
    constexpr const_iterator cend() const noexcept { /* TODO */ }

    constexpr bool empty() const noexcept { /* TODO */ }
    constexpr size_type size() const noexcept { /* TODO */ }
    constexpr size_type max_size() const noexcept { /* TODO */ }
    constexpr void reserve(size_type new_cap) { /* TODO */ }
    constexpr size_type capacity() const noexcept { /* TODO */ }
    constexpr void shrink_to_fit() { /* TODO */ }
    
    private:
        pointer m_data;
        size_type m_size;
        size_type m_capacity;
        [[no_unique_address]] allocator_type m_alloc;
};