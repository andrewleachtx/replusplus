#include <gtest/gtest.h>
#include <replusplus/boundedworkqueue.hpp>

#include <future>

const auto MAX_THREADS = std::thread::hardware_concurrency();

using Job = replusplus::WorkQueue::Job;
void func() { printf("job exec!\n"); }

TEST(BoundedWorkQueueTests, SerialDoesJob) {
    constexpr std::size_t worker_ct = 1;

    // get_future tied to this promise says that when the producer (job_executed) updates the shared state,
    // the future can be blocked on until it happens. answer_promise.set_value(42), answer_future.get()
    // should return 42 when the former occurs.
    std::promise<void> job_executed;

    // Get the future associated with this promise.
    auto job_executed_future = job_executed.get_future();

    {
        replusplus::WorkQueue wq(worker_ct);

        // Push a job which should simply update this promise.
        wq.push(Job([&job_executed]() { job_executed.set_value(); }));
    }

    // After destruction/cleanup occurs, we should expect the promise to be complete.
    ASSERT_EQ(job_executed_future.wait_for(std::chrono::seconds{0}),
              std::future_status::ready);

    EXPECT_NO_THROW(job_executed_future.get());
}