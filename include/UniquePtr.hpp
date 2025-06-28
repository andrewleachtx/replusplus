/*
    https://en.cppreference.com/w/cpp/memory/unique_ptr.html

    A stack allocated object wrapper that stores a raw pointer to an object (RAII) and manages its lifetime.
    
    Notably, when the object is gone, the object is too. Not using the "class Deleter" thing.
*/

// TODO: Fix func signatures
template <class T>
class UniquePtr {
    public:
        // Constructors
        constexpr UniquePtr() noexcept { /* TODO */ }
        explicit UniquePtr(T *ptr) noexcept { /* TODO */}
        UniquePtr(const UniquePtr &other) noexcept { /* TODO */ }
        UniquePtr(UniquePtr &&other) noexcept { /* TODO */ }
        
        // Destructor
        ~UniquePtr() { /* TODO */ }
        
        // Assignment
        UniquePtr& operator=(const UniquePtr &other) = delete;
        UniquePtr& operator=(UniquePtr &&other) noexcept { /* TODO */ }
        
        T* release() noexcept { /* TODO */}
        // T*() == nullptr...
        constexpr void reset(T *ptr=T*()) noexcept { /* TODO */ }
        void swap(UniquePtr &other) noexcept { /* TODO */ }

        T* get() const noexcept { /* TODO */ }
        explicit operator bool() const noexcept { /* TODO */ }

        T* operator->() const noexcept { /* TODO */ }

    private:
        T *m_ptr;
};

template<class T, class... Args>
UniquePtr<T> make_unique(Args&&... args) {/* TODO */}