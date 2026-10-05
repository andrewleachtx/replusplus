#include <replusplus/condition_variable.hpp>
#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

TEST(ConditionVariableTests, TruePredicateDoesNotBlock) {
    replusplus::condition_variable cv;
    std::mutex mutex;
    std::unique_lock lock(mutex);
    bool ready = true;

    cv.wait(lock, [&ready]() { return ready; });

    EXPECT_TRUE(lock.owns_lock());
}

TEST(ConditionVariableTests, NotifyOneWakesAWaiter) {
    replusplus::condition_variable cv;
    std::mutex mutex;
    bool ready = false;

    std::promise<void> waiter_entered;
    std::promise<void> waiter_finished;
    auto entered = waiter_entered.get_future();
    auto finished = waiter_finished.get_future();

    std::thread waiter([&]() {
        std::unique_lock lock(mutex);
        waiter_entered.set_value();

        cv.wait(lock, [&ready]() { return ready; });
        waiter_finished.set_value();
    });

    entered.wait();
    {
        std::scoped_lock lock(mutex);
        ready = true;
    }
    cv.notify_one();

    const auto first_notification = finished.wait_for(1s);

    // Ensure a failing test can still clean up its worker thread.
    if (first_notification != std::future_status::ready) {
        cv.notify_all();
    }

    waiter.join();
    EXPECT_EQ(first_notification, std::future_status::ready);
}

TEST(ConditionVariableTests, NotifyAllWakesEveryWaiter) {
    constexpr std::size_t waiter_count = 4;

    replusplus::condition_variable cv;
    std::mutex mutex;
    bool ready = false;

    std::vector<std::promise<void>> entered_promises(waiter_count);
    std::vector<std::future<void>> entered_futures;
    std::vector<std::promise<void>> finished_promises(waiter_count);
    std::vector<std::future<void>> finished_futures;
    std::vector<std::thread> waiters;

    entered_futures.reserve(waiter_count);
    finished_futures.reserve(waiter_count);
    waiters.reserve(waiter_count);

    for (std::size_t i = 0; i < waiter_count; ++i) {
        entered_futures.push_back(entered_promises[i].get_future());
        finished_futures.push_back(finished_promises[i].get_future());

        waiters.emplace_back([&, i]() {
            std::unique_lock lock(mutex);
            entered_promises[i].set_value();

            cv.wait(lock, [&ready]() { return ready; });
            finished_promises[i].set_value();
        });
    }

    for (auto& entered : entered_futures) {
        entered.wait();
    }

    {
        std::scoped_lock lock(mutex);
        ready = true;
    }
    cv.notify_all();

    bool everyone_woke = true;
    for (auto& finished : finished_futures) {
        if (finished.wait_for(1s) != std::future_status::ready) {
            everyone_woke = false;
        }
    }

    // Ensure a failing test can still clean up any remaining worker threads.
    if (!everyone_woke) {
        cv.notify_all();
    }

    for (auto& waiter : waiters) {
        waiter.join();
    }

    EXPECT_TRUE(everyone_woke);
}

TEST(ConditionVariableTests, NotificationBeforeWaitIsNotRemembered) {
    replusplus::condition_variable cv;
    std::mutex mutex;

    cv.notify_one();

    std::promise<void> waiter_entered;
    std::promise<void> waiter_finished;
    auto entered = waiter_entered.get_future();
    auto finished = waiter_finished.get_future();

    std::thread waiter([&]() {
        std::unique_lock lock(mutex);
        waiter_entered.set_value();
        cv.wait(lock);
        waiter_finished.set_value();
    });

    entered.wait();

    // Acquiring this mutex proves the waiter released it inside wait().
    {
        std::scoped_lock lock(mutex);
    }

    EXPECT_EQ(finished.wait_for(50ms), std::future_status::timeout);

    cv.notify_one();
    const auto second_notification = finished.wait_for(1s);

    // Do not leave a joinable thread behind if the assertion fails.
    if (second_notification != std::future_status::ready) {
        cv.notify_all();
    }

    waiter.join();
    EXPECT_EQ(second_notification, std::future_status::ready);
}
