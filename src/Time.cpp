/**
 * @file Time.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of Time utilites
 * @version 0.1
 * @date 2024-11-09
 */
#include "Time.h"

#include <iostream>
#include <chrono>

using namespace EBPS;

#define SEC_TO_MICRO 1000

uint32_t EBPS::Time::now() {
    // std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    // std::chrono::system_clock::now().time_since_epoch());

    // return ms.count();
    return 1;
}

uint32_t EBPS::Time::secToMicro(const uint32_t sec) {
    return sec * SEC_TO_MICRO;
}

int main(void) {
    uint32_t ms = EBPS::Time::now();
    // std::cout << "Time now = " << ms << "\n";
}