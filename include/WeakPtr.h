#pragma once

template <typename T>
struct ControlBlock;

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr {
    public:
        constexpr WeakPtr() noexcept {}
        WeakPtr(const &WeakPtr other) noexcept {}
        WeakPtr(const &SharedPtr other) noexcept {}
        SharedPtr<T> lock() const noexcept {
            return SharedPtr<T>(m_ctrlBlk);
        }
    
    private:
        ControlBlock<T> *m_ctrlBlk = nullptr;
};