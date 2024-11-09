#include <catch2/catch_all.hpp>

#include "Scheduler.h"

using namespace EBPS;

TEST_CASE("Scheduler can queue a task", "[Scheduler]") {
    Scheduler scheduler;

    int count = 0;

    scheduler.createTimeout([&]() {count++;}, 100);

    scheduler.start();

    REQUIRE(count == 1);
}