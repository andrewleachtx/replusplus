#include <gtest/gtest.h>
#include <replusplus/boundedworkqueue.hpp>

#include <atomic>
#include <cstddef>
#include <future>
#include <vector>

using Job = replusplus::WorkQueue::Job;

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

TEST(BoundedWorkQueueTests, DrainsAllQueuedJobsOnDestruction) {
    constexpr std::size_t job_count = 100;
    std::atomic<std::size_t> completed_jobs = 0;

    {
        replusplus::WorkQueue wq(4);

        for (std::size_t i = 0; i < job_count; ++i) {
            wq.push([&completed_jobs]() { ++completed_jobs; });
        }
    }

    EXPECT_EQ(completed_jobs.load(), job_count);
}

TEST(BoundedWorkQueueTests, SingleWorkerExecutesJobsInFifoOrder) {
    constexpr std::size_t job_count = 10;
    std::vector<std::size_t> execution_order;

    {
        replusplus::WorkQueue wq(1);

        for (std::size_t i = 0; i < job_count; ++i) {
            wq.push([&execution_order, i]() { execution_order.push_back(i); });
        }
    }

    ASSERT_EQ(execution_order.size(), job_count);
    for (std::size_t i = 0; i < job_count; ++i) {
        EXPECT_EQ(execution_order[i], i);
    }
}

TEST(BoundedWorkQueueTests, LimitsConcurrentJobsToWorkerCount) {
    constexpr std::size_t worker_count = 2;
    constexpr std::size_t job_count = 6;

    std::atomic<std::size_t> started_jobs = 0;
    std::atomic<std::size_t> completed_jobs = 0;

    std::promise<void> release_jobs;
    const std::shared_future<void> release_gate =
        release_jobs.get_future().share();

    std::promise<void> both_workers_started;
    auto both_workers_started_future = both_workers_started.get_future();

    {
        replusplus::WorkQueue wq(worker_count);

        for (std::size_t i = 0; i < job_count; ++i) {
            wq.push([&started_jobs, &completed_jobs, &both_workers_started,
                     release_gate]() {
                const std::size_t started = ++started_jobs;
                if (started == worker_count) {
                    both_workers_started.set_value();
                }

                release_gate.wait();
                ++completed_jobs;
            });
        }

        const auto status =
            both_workers_started_future.wait_for(std::chrono::seconds{1});
        const std::size_t jobs_started_while_blocked = started_jobs.load();

        // Always release any workers before an assertion can leave this scope.
        release_jobs.set_value();

        ASSERT_EQ(status, std::future_status::ready);
        EXPECT_EQ(jobs_started_while_blocked, worker_count);
    }

    EXPECT_EQ(started_jobs.load(), job_count);
    EXPECT_EQ(completed_jobs.load(), job_count);
}
