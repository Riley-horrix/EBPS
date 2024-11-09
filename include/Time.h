/**
 * @file Time.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of a Time utility class.
 * @version 0.1
 * @date 2024-11-09
 */
#pragma once

#include <cinttypes>

namespace EBPS {

class Time {
public:
    /**
     * @brief Returns the current time since clock epoch in milliseconds.
     * 
     * @return uint64_t Time since epoch in millis.
     */
    static uint32_t now(void);

    /**
     * @brief Convert seconds to milliseconds.
     */
    static inline uint32_t secToMicro(const uint32_t sec);

private:
    // Delete constructors and destructors
    // Time() = delete;
    // ~Time() = delete;
};

} // Namespace EBPS