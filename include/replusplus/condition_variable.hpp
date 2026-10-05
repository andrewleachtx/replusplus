#include <atomic>
#include <replusplus/mutex.hpp>

#include <mutex>
#include <thread>

namespace replusplus {
class condition_variable {
public:
    using thread_id = decltype(std::this_thread::get_id());

    condition_variable() {}
    ~condition_variable() {}

    condition_variable(const condition_variable&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;

    void notify_one() noexcept {
        // Our generation counter has incremented, notify one thread spinning that they can
        generation_.fetch_add(1, std::memory_order_release);
        generation_.notify_one();
    }
    void notify_all() noexcept {
        generation_.fetch_add(1, std::memory_order_release);
        generation_.notify_all();
    }

    void wait(std::unique_lock<std::mutex>& lock) {
        std::uint64_t initial_ct = generation_.load(std::memory_order_acquire);
        
        // Relinquish the lock only after grabbing the atomic
        lock.unlock();
        
        // Wait until a change has happened
        generation_.wait(initial_ct, std::memory_order_acquire);

        // If we make it here, our thread has been notified, and the value underlying generation_ 
        // has changed - we should exit our wait and reacquire the lock
        lock.lock();
    }

    template <typename Predicate>
    void wait(std::unique_lock<std::mutex>& lock, Predicate pred) {
        // Call unlock and block until the condition variable is notified or a spurious wakeup occurs.
        while (!pred()) {
            // Even if our wait allows exit, if the predicate is not true enter back in wait, relinquishing
            // the lock again
            wait(lock);
        }
    }

private:
    std::atomic<std::uint64_t> generation_{0};
};
} // namespace replusplus