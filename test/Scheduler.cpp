#include <catch2/catch_all.hpp>

#include "Scheduler.h"

using namespace EBPS;

TEST_CASE("Scheduler can queue a task", "[Scheduler]") {
    Scheduler scheduler;

    int count = 0;

    // Schedule timeout to inc counter
    scheduler.timeout([&]() {count++;}, 10);

    scheduler.start();

    REQUIRE(count == 1);

}

TEST_CASE("Task can stop scheduler and will run again", "[Scheduler]") {
    Scheduler scheduler;

    int count = 0;

    // Schedule an earlier timeout that stops the scheduler
    scheduler.timeout([&]() {count++; scheduler.stop();}, 10);
    scheduler.timeout([&]() {count++;}, 20);

    // Timeout 1 should stop scheduler after incrementing counter
    scheduler.start();
    REQUIRE(count == 1);

    // Starting again should restart and run remaining tasks
    scheduler.start();
    REQUIRE(count == 2);
}

TEST_CASE("Scheduler will order timeouts correctly", "[Scheduler]") {
    Scheduler scheduler;

    int a = 0;
    int b = 0;
    int c = 0;

    scheduler.timeout([&]() {
        b++;
        REQUIRE((a==1 && b == 1 && c == 0));
    }, 20);

    scheduler.timeout([&]() {
        a++;
        REQUIRE((a==1 && b == 0 && c == 0));
    }, 10);

    scheduler.timeout([&]() {
        c++;
        REQUIRE((a==1 && b == 1 && c == 1));
    }, 30);

    scheduler.start();
    REQUIRE(((a == b) && (c == 1) && (b == c)));
}

TEST_CASE("Tasks can be cancelled", "[Scheduler]") {
    Scheduler scheduler;

    int count = 0;

    // Shouldn't run and count should be 0
    auto handle = scheduler.timeout([&]() {
        count++;
    }, 20);

    scheduler.timeout([&]() {
        handle->cancel();
    }, 10);

    scheduler.start();
    REQUIRE(count == 0);

    // Cancelling yourself causes nothing weird and should run to completion
    std::unique_ptr<Scheduler::Handle> handle2 = scheduler.timeout([&]() {
        handle2->cancel();
        count++;
    }, 0);

    scheduler.start();

    REQUIRE(count == 1);
}

TEST_CASE("Scheduler can repeat interval tasks", "[Scheduler]") {
    Scheduler scheduler;

    int count = 0;
    // Schedule interval to inc counter 10 times
    std::unique_ptr<Scheduler::Handle> handle = scheduler.interval([&]() {
        count++;
        if (count == 10) {
            handle->cancel();
        }
    }, 10);

    scheduler.start();
    REQUIRE(count == 10);
}