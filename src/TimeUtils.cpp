/**
 * @file Time.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of Time utilites
 * @version 0.1
 * @date 2024-11-09
 */
#include "TimeUtils.h"

#include <cstdint>
#include <chrono>

using namespace EBPS;

const int SEC_TO_MILLI = 1000;

EBPS::time_t TimeUtils::now() {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());

    return ms.count();
}

EBPS::time_t TimeUtils::secToMilli(const EBPS::time_t sec) {
    return sec * SEC_TO_MILLI;
}