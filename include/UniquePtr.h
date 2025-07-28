#pragma once

#include <utility>

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
        constexpr UniquePtr() noexcept : m_ptr(nullptr) {}
        explicit UniquePtr([[maybe_unused]] T *ptr) noexcept : m_ptr(nullptr) {}
        UniquePtr(const UniquePtr &other) = delete;
        UniquePtr(UniquePtr &&other) noexcept {
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        
        // Destructor
        ~UniquePtr() {
            reset();
        }
        
        // Assignment
        UniquePtr& operator=(const UniquePtr &other) = delete;
        UniquePtr& operator=(UniquePtr &&other) noexcept {
            if (this != &other) {
                delete m_ptr;
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }

            return *this;
        }
        
        T* release() noexcept {
            T* ptrCpy = m_ptr;
            m_ptr = nullptr;
            return ptrCpy;
        }
        
        // T*() == nullptr...
        constexpr void reset(T *ptr=nullptr) noexcept {
            if (m_ptr != ptr) {
                delete m_ptr;
                m_ptr = ptr;
            }
        }

        void swap(UniquePtr &other) noexcept {
            std::swap(m_ptr, other.m_ptr);
        }

        T* get() const noexcept {
            return m_ptr;
        }
        explicit operator bool() const noexcept {
            return m_ptr != nullptr;
        }

        // "If get() is a nullptr, the behavior is undefined."
        T& operator*() const noexcept {
            return *m_ptr;
        }
        T* operator->() const noexcept {
            return m_ptr;
        }

    private:
        T *m_ptr;
};

template<class T, class... Args>
UniquePtr<T> MakeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}