#include <catch2/catch_all.hpp>

#include "TimeUtils.h"

using namespace EBPS;

TEST_CASE("Time gets correct time", "[Time]") {
    REQUIRE(TimeUtils::secToMilli(1) == 1000);
    REQUIRE(TimeUtils::secToMilli(150) == 150000);
}