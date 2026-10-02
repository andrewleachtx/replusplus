#include <atomic>

namespace replusplus {
class mutex {
public:
    mutex() = default;
    mutex& operator=(const mutex&) = delete;
    mutex& operator=(mutex&&) = delete;
    mutex(const mutex&) = delete;
    mutex(mutex&&) = delete;
    
    /*
        atomic.compare_exchange_strong(T& expected, T desired) {
            if atomic == expected:
                atomic = desired
                return true (a store happened)
            else:
                expected = atomic
                return false (no store happened)
        }

        TODO: yield to CPU instead of spinlock
        TODO: what memory ordering to use?
    */
    void lock() {
        bool expected = false;

        // Try to acquire the lock
        while (!is_locked_.compare_exchange_strong(expected, true)) {
            expected = false;
        }
    }
    void unlock() {
        is_locked_.store(false);
    }

private:
    std::atomic<bool> is_locked_ { false };
};
} // namespace replusplus