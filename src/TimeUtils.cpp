/**
 * @file Time.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of Time utilites
 * @version 0.1
 * @date 2024-11-09
 */
#include "TimeUtils.h"

#include <cstdint>
#include <iostream>
#include <chrono>

using namespace EBPS;

#define SEC_TO_MILLI 1000

uint32_t TimeUtils::now() {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());

    return ms.count();
}

uint32_t TimeUtils::secToMilli(const uint32_t sec) {
    return sec * SEC_TO_MILLI;
}