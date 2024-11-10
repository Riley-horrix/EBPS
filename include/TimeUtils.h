/**
 * @file Time.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of a Time utility class.
 * @version 0.1
 * @date 2024-11-09
 */
#pragma once

#include <cstdint>

namespace EBPS {

//! @brief Time type - this is 32 bits because the ESP32 only gives 32 bits of
//! accuracy.
using time_t = uint32_t;

/**
 * @brief A utility class providing time based functions and utilities.
 * 
 * Mainly, it can give the current 'ground truth' time, this is not defined as
 * being the time since epoch (since it is only 32b). In the future if the 
 * system can be configured to query GPS time, the time_t type can be changed
 * to a 64 bit type, and a constant offset can be applied to the system clock.
 * 
 */
class TimeUtils {
public:
    /**
     * @brief Returns the current time since clock epoch in milliseconds.
     * 
     * @return time_t Time since epoch in millis.
     */
    static time_t now(void);

    /**
     * @brief Convert seconds to milliseconds.
     */
    static time_t secToMilli(const time_t sec);

private:
    // Delete constructors and destructors
    TimeUtils() = delete;
    ~TimeUtils() = delete;
};

} // Namespace EBPS