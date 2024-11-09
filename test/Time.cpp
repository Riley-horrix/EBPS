// #include <catch2/catch_all.hpp>

#include "Time.h"

#include <chrono>

using namespace EBPS;

// TEST_CASE("Time gets correct time", "[Time]") {

// }


void func () {
    const auto& timeMicros = std::chrono::system_clock::now();
}