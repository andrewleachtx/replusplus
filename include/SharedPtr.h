#pragma once
#include <atomic>

template<typename T> struct ControlBlock {
    T* ptr;
    std::atomic<std::size_t> strongCt;
    std::atomic<std::size_t> weakCt;

    ControlBlock() : ptr(nullptr), strongCt(0), weakCt(0) {}
    ControlBlock(T* p) : ptr(p), strongCt(1), weakCt(1) {}
    ~ControlBlock() {}
};

/*
    Although double delete is a thing, SharedPtr(T *rawPtr) exists -- primarily for MakeShared?
*/
template<typename T> class SharedPtr {
  public:
    constexpr SharedPtr() noexcept : m_ctrlBlk(nullptr) {}
    constexpr SharedPtr(std::nullptr_t) noexcept : m_ctrlBlk(nullptr) {}
    explicit SharedPtr(T* rawPtr) noexcept : m_ctrlBlk(new ControlBlock(rawPtr)) {}
    SharedPtr(const SharedPtr& other) noexcept {
        m_ctrlBlk = other.m_ctrlBlk;
        m_ctrlBlk->strongCt.fetch_add(1);
    }
    SharedPtr(SharedPtr&& other) noexcept {
        m_ctrlBlk = other.m_ctrlBlk;
        other.m_ctrlBlk = nullptr;
    }
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            release();

            m_ctrlBlk = other.m_ctrlBlk;
            m_ctrlBlk->strongCt.fetch_add(1);
        }

        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();

            m_ctrlBlk = other.m_ctrlBlk;
            other.m_ctrlBlk = nullptr;
        }

        return *this;
    }

    ~SharedPtr() { release(); }

    void reset() noexcept { release(); }
    void reset(T* ptr) {
        release();

        if (ptr) {
            m_ctrlBlk = new ControlBlock(ptr);
        }
    }

    void swap(SharedPtr& other) noexcept { std::swap(m_ctrlBlk, other.m_ctrlBlk); }

    T* get() const noexcept {
        if (!m_ctrlBlk) {
            return nullptr;
        }

        return m_ctrlBlk->ptr;
    }

    T& operator*() const noexcept { return *get(); }
    T* operator->() const noexcept { return get(); }

    std::size_t use_count() const noexcept {
        if (!m_ctrlBlk) {
            return 0;
        }

        return m_ctrlBlk->strongCt.load(std::memory_order_relaxed);
    }

    bool unique() const noexcept { return use_count() == 1; }

    explicit operator bool() const noexcept { return get() != nullptr; }

  private:
    ControlBlock<T>* m_ctrlBlk;
    explicit SharedPtr(ControlBlock<T>* ctrlBlk) noexcept : m_ctrlBlk(ctrlBlk) {
        // TODO:
    }

    // Called from destructor
    void release() {
        if (m_ctrlBlk) {
            if (m_ctrlBlk->strongCt.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete m_ctrlBlk->ptr;

                if (m_ctrlBlk->weakCt.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                    delete m_ctrlBlk;
                }
            }
            m_ctrlBlk = nullptr;
        }
    }
};

template<typename T, typename... Args> SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}