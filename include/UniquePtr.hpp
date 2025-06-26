/*
    https://en.cppreference.com/w/cpp/memory/unique_ptr.html

    A stack allocated object wrapper that stores a raw pointer to an object (RAII) and manages its lifetime.
    
    Notably, when the object is gone, the object is too. Not using the "class Deleter" thing.
*/

// TODO: Fix func signatures
template <class T>
class UniquePtr {
    public:
        constexpr UniquePtr() noexcept { /* TODO */ }
        UniquePtr(const UniquePtr &other) { /* TODO */ }
        UniquePtr(UniquePtr &&other) = delete;
        operator=(const UniquePtr &other);
        operator=(UniquePtr &&other) = delete;
        ~UniquePtr() {
            delete T;
        }
        
        T* release() noexcept { /* TODO */}
        void reset(T *ptr=T()) noexcept { /* TODO */ } // This signature looks weird https://en.cppreference.com/w/cpp/memory/unique_ptr/reset.html
        

    private:
        T *m_ptr;
};