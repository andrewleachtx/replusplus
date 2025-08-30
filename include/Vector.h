#include <initializer_list>
#include <limits>
#include <memory>

const size_t GROWTH_FACTOR = 2UL;

template<typename T, typename Allocator = std::allocator<T>> class Vector {
  public:
    struct Iterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        Iterator() = default;
        explicit Iterator(pointer ptr) : m_ptr(ptr) {}
        Iterator &operator=(const Iterator &) noexcept = default;

        [[nodiscard]] reference operator*() const { return *m_ptr; }
        [[nodiscard]] pointer operator->() const { return m_ptr; }

        Iterator &operator++() noexcept {
            ++m_ptr;
            return *this;
        }
        Iterator operator++(int) noexcept {
            Iterator cpy = *this;
            m_ptr++;
            return cpy;
        }
        Iterator &operator--() noexcept {
            --m_ptr;
            return *this;
        }
        Iterator operator--(int) noexcept {
            Iterator cpy = *this;
            m_ptr--;
            return cpy;
        }

        [[nodiscard]] Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
        [[nodiscard]] Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }
        difference_type operator-(const Iterator &rhs) { return m_ptr - rhs.m_ptr; }

        // TODO: Learn how to define three way comparison operator manually
        auto operator<=>(const Iterator &rhs) const = default;

        pointer m_ptr = nullptr;
    };

    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    using traits = std::allocator_traits<allocator_type>;

    Vector() = default;
    explicit Vector(size_type count, const allocator_type &alloc = allocator_type())
        : m_data(nullptr), m_size(count), m_capacity(count), m_alloc(alloc) {
        m_data = traits::allocate(m_alloc, count);

        for (size_type i = 0; i < count; i++) {
            // Anywhere we do m_data + i, we could also do &m_data[i] if it is more intuitive.
            traits::construct(m_alloc, m_data + i, value_type());
        }
    }
    constexpr Vector(size_type count, const_reference value, const allocator_type &alloc = allocator_type())
        : m_data(nullptr), m_size(count), m_capacity(count), m_alloc(alloc) {
        m_data = traits::allocate(m_alloc, count);

        for (size_type i = 0; i < count; i++) {
            traits::construct(m_alloc, m_data + i, value);
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
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity), m_alloc(std::move(other.m_alloc)) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_alloc = allocator_type{};
    }

    ~Vector() { reset(); }

    constexpr Vector &operator=(const Vector &other) {
        if (this != &other) {
            // Assume we need to destroy all of our values even if we don't
            reset();

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
    Vector &operator=(Vector &&other) noexcept {
        if (this != &other) {
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_alloc = std::move(other.m_alloc);

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_alloc = allocator_type{};
        }

        return *this;
    }
    Vector &operator=(std::initializer_list<value_type> ilist) {
        // Destroy and dealloc
        reset();

        insert(this->end(), ilist.begin(), ilist.end());
    }

    constexpr void assign(size_type count, const_reference value) {
        // Destruct existing elements (dont dealloc)
        clear();

        // Check if we can fit room, if not realloc and move
        reserve(count);

        for (size_type i = 0; i < count; i++) {
            traits::construct(m_alloc, m_data + i, value);
        }
        m_size = count;
    }
    constexpr void assign(std::initializer_list<value_type> ilist) {
        clear();
        reserve(ilist.size());

        for (auto it = ilist.begin(); it != ilist.end(); it++) {
            size_type i = it - ilist.begin();
            traits::construct(m_alloc, m_data + i, *it);
        }
        m_size = ilist.size();
    }

    constexpr allocator_type get_allocator() const { return m_alloc; }

    constexpr reference at(size_type pos) {
        if (pos >= m_size) {
            throw std::out_of_range("Out of bounds access in .at(...)!");
        }
        return m_data[pos];
    }
    constexpr const_reference at(size_type pos) const {
        if (pos >= m_size) {
            throw std::out_of_range("Out of bounds access in .at(...)!");
        }
        return m_data[pos];
    }

    constexpr reference operator[](size_type pos) { return m_data[pos]; }
    constexpr const_reference operator[](size_type pos) const { return m_data[pos]; }

    constexpr reference front() { return m_data[0]; }
    constexpr const_reference front() const { return m_data[0]; }

    constexpr reference back() { return m_data[m_size - 1]; }
    constexpr const_reference back() const { return m_data[m_size - 1]; }

    constexpr pointer data() noexcept { return m_data; }
    constexpr const_pointer data() const noexcept { return m_data; }

    constexpr iterator begin() noexcept { return iterator(m_data); }
    constexpr const_iterator begin() const noexcept { return const_iterator(m_data); }
    constexpr const_iterator cbegin() const noexcept { return const_iterator(m_data); }

    constexpr iterator end() noexcept { return iterator(m_data + m_size); }
    constexpr const_iterator end() const noexcept { return const_iterator(m_data + m_size); }
    constexpr const_iterator cend() const noexcept { return const_iterator(m_data + m_size); }

    constexpr bool empty() const noexcept { return m_size == 0; }
    constexpr size_type size() const noexcept { return m_size; }
    // Even if we theoretically have numbers bigger than size_t for memory, if we had to make a call of malloc with
    // size_t nbytes = sizeof(T) * n, nbytes would exceed size_t
    constexpr size_type max_size() const noexcept {
        // return std::numeric_limits<size_type>::max() / sizeof(value_type);
        return traits::max_size(m_alloc);
    }
    constexpr void reserve(size_type new_cap) {
        if (new_cap > max_size()) {
            throw std::length_error("New capacity exceeds maximum size!");
        }
        if (new_cap <= m_capacity) {
            return;
        }

        // Reallocates new space of new_cap and std::moves old values into it.
        reallocAndMove(new_cap);
    }
    constexpr size_type capacity() const noexcept { return m_capacity; }
    constexpr void shrink_to_fit() { reallocAndMove(m_size); }

    constexpr void clear() noexcept {
        for (size_t i = 0; i < m_size; i++) {
            traits::destroy(m_alloc, m_data + i);
        }

        m_size = 0;
    }

    /*
        Insert should try to find the said position, insert value, and shift
    */
    constexpr iterator insert(const_iterator pos, const value_type &value) {
        growIfNeeded();

        difference_type posIdx = pos - cbegin();

        // Shift all values over
        for (size_type i = m_size; i > static_cast<size_type>(posIdx); i--) {
            traits::construct(m_alloc, m_data + i, std::move(m_data[i - 1]));
            traits::destroy(m_alloc, m_data + i - 1);
        }

        // Insert value at pos
        traits::construct(m_alloc, m_data + posIdx, value);
        m_size++;

        return iterator(m_data + posIdx);
    }
    constexpr iterator insert(const_iterator pos, value_type &&value) {
        growIfNeeded();

        difference_type posIdx = pos - cbegin();

        // Shift all values over
        for (size_type i = m_size; i > static_cast<size_type>(posIdx); i--) {
            traits::construct(m_alloc, m_data + i, std::move(m_data[i - 1]));
            traits::destroy(m_alloc, m_data + i - 1);
        }

        // Insert value at pos
        traits::construct(m_alloc, m_data + posIdx, std::move(value));
        m_size++;

        return iterator(m_data + posIdx);
    }
    constexpr iterator insert(const_iterator pos, size_type count, const value_type &value) {
        growIfNeeded(count);

        difference_type posIdx = pos - cbegin();

        // Shift all elements over
        for (size_type i = m_size + count - 1; i >= static_cast<size_type>(posIdx) + count; i--) {
            traits::construct(m_alloc, m_data + i, std::move(m_data[i - count]));
            traits::destroy(m_alloc, m_data + i - count);
        }

        // Insert count copies of value
        for (size_type i = 0; i < count; i++) {
            traits::construct(m_alloc, m_data + posIdx + i, value);
        }

        m_size += count;
        return iterator(m_data + posIdx);
    }
    constexpr iterator insert(const_iterator pos, const_iterator first, const_iterator last) {
        size_type count = static_cast<size_type>(last - first);

        growIfNeeded(count);

        difference_type posIdx = pos - cbegin();

        for (size_type i = m_size + count - 1; i >= static_cast<size_type>(posIdx) + count; i--) {
            traits::construct(m_alloc, m_data + i, std::move(m_data[i - count]));
            traits::destroy(m_alloc, m_data + i - count);
        }

        auto it = first;
        for (size_type i = 0; i < count; i++, it++) {
            traits::construct(m_alloc, m_data + posIdx + i, *it);
        }

        m_size += count;
        return iterator(m_data + posIdx);
    }

    template<typename... Args> constexpr iterator emplace(const_iterator, Args &&... args) { /* TODO */
    }

    constexpr iterator erase(iterator pos) { /* TODO */
    }
    constexpr iterator erase(iterator first, iterator last) { /* TODO */
    }

    constexpr void push_back(const T &value) { /* TODO */
    }
    constexpr void push_back(T &&value) { /* TODO */
    }

    template<typename... Args> constexpr void emplace_back(Args &&... args) { /* TODO */
    }
    template<typename... Args> constexpr reference emplace_back(Args &&... args) { /* TODO */
    }

    // FIXME: Maybe we don't throw, is UB?
    constexpr void pop_back() {
        if (m_size == 0) {
            throw std::runtime_error("pop_back() called on empty vector!");
        }

        m_size--;
    }

    // FIXME: Can this shrink the array? I think it can, so reallocAndMove() accounts for that
    constexpr void resize(size_type count) { reallocAndMove(count); }
    constexpr void resize(size_type count, const value_type &value) {
        // TODO: Could add this overload, but lazy
        // reallocAndMove(count, value);
        reallocAndMove(count);
        for (size_type i = 0; i < count; i++) {
            m_data[i] = value;
        }
    }

    constexpr void swap(Vector &other) noexcept { /* TODO */
    }

  private:
    pointer m_data;
    size_type m_size;
    size_type m_capacity;
    [[no_unique_address]] allocator_type m_alloc;

    // Maybe make a void, but this can give you info
    constexpr bool growIfNeeded(size_type numInserts = 1) {
        size_type proposedSize = m_size + numInserts;
        if (proposedSize > m_capacity) {
            // For inserts that take multiple doubles we can avoid unnecessary allocs by doing in one call; find
            // necessary new capacity
            size_type grownCapacity = m_capacity == 0 ? 1 : m_capacity * GROWTH_FACTOR;
            size_type newCapacity = std::max(proposedSize, grownCapacity);
            reallocAndMove(newCapacity);

            return true;
        }

        return false;
    }

    constexpr void reallocAndMove(size_type new_cap) {
        pointer new_data = traits::allocate(m_alloc, new_cap);

        /*
            In the case where new_cap < m_size (probably a resize call), we should avoid segfaulting by only
            constructing spots that are in range.
        */
        size_t lesser_size = std::min(m_size, new_cap);

        for (size_type i = 0; i < lesser_size; i++) {
            traits::construct(m_alloc, new_data + i, std::move(m_data[i]));
        }

        // Still need to destroy the old array
        for (size_type i = 0; i < m_size; i++) {
            traits::destroy(m_alloc, m_data + i);
        }

        traits::deallocate(m_alloc, m_data, m_capacity);

        m_data = new_data;
        m_size = lesser_size;
        m_capacity = new_cap;
    }

    /*
        This is clear but deallocates and destroys memory. Could add a flag in clear for this as well, but users
       shouldn't see that.

        It comes as the cost of usually having some repeated instructions (e.g., m_data set to null to only be set to
       new after, etc.) but poor readability/DRY is a cost itself too.
    */
    constexpr void reset() {
        for (size_type i = 0; i < m_size; i++) {
            traits::destroy(m_alloc, m_data + i);
        }

        if (m_data) {
            traits::deallocate(m_alloc, m_data, m_capacity);
        }

        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
        m_alloc = allocator_type{};
    }
};