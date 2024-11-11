/**
 * @file Message.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of the Message base class.
 * @version 0.1
 * @date 2024-11-11
 */
#pragma once

#include <cstdint>

namespace EBPS {

using msgid_t = uint8_t;

class Message {
public:
    /**
     * @brief Construct a new Message object
     */
    Message();

    /**
     * @brief Destroy the Message object.
     */
    virtual ~Message() = default;

    /**
     * @brief Unique identifier for this message type.
     * 
     * Note that this id is not unique among messages of same type.
     */
    const msgid_t id = 0;
};

}