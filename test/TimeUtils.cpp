#include <catch2/catch_all.hpp>

#include "TimeUtils.h"

#include <iostream>

using namespace EBPS;

TEST_CASE("Time gets correct time", "[Time]") {
    std::cout << "[Time] tests\n";
    REQUIRE(TimeUtils::secToMilli(1) == 1000);
    REQUIRE(TimeUtils::secToMilli(150) == 150000);
}
