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

//! @brief Time type
using time_t = uint32_t;

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