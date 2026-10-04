#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>


namespace replusplus {
class WorkQueue {
public:
    using Job = std::function<void()>;

    explicit WorkQueue(const std::size_t width) {
        consumers_.reserve(width);
        // TODO is there a way to default construct this properly? or resize?
        for (std::size_t i = 0; i < width; i++) {
            consumers_.push_back(std::thread{[this]() { pop(); }});
        }
    }
    ~WorkQueue() {
        is_destroying_ = true;
        for (auto& thr : consumers_) {
            thr.join();
        }
    }

    void push(Job job) {
        std::scoped_lock lock(mutex_);
        waiting_jobs_.push(job);
        cv_.notify_one();
    }

private:
    std::queue<Job> waiting_jobs_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool is_destroying_ = false;

    // Need this last, because instantiation happens in order of declaration
    std::vector<std::thread> consumers_;

    void pop() {
        // Threads should spin in here waiting to take on a new job.
        while (!is_destroying_) {
            std::unique_lock lock(mutex_);

            // Condition variables signal the thread to wake up when the predicate becomes true
            cv_.wait(lock, [this]() { return !waiting_jobs_.empty(); });

            // Critical section
            Job job = waiting_jobs_.front();
            waiting_jobs_.pop();

            lock.unlock();

            // Execute job
            job();
        }
    }
};
} // namespace replusplus