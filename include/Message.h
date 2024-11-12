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


/**
 * @brief Message Base Class.
 * 
 * Holds an id field that is set by the derived class in the constructor.
 * 
 * Example usage :
 * 
 * @code {.cpp}
 * class GyroMessage : Message {
 * public:
 *   GyroMessage(double x, double y, double z):
 *      Message(GyroMessage::id), accx(x), accy(y), accz(z) {}
 *   
 *   ~GyroMessage() {}
 * 
 *   double accx = 0;
 *   double accy = 0;
 *   double accz = 0;
 * 
 *   static const msgid_t id = GYRO_MSG;
 * }
 * @endcode
 * 
 * Then to access the id field from an erased type :
 * 
 * @code {.cpp}
 * 
 * std::shared_ptr<Message> message;
 * 
 * if (message->id == GyroMessage::id) {
 *   // Handle gyro message
 * }
 * @endcode
 * 
 * 
 */
class Message {
public:
    /**
     * @brief Unique identifier for this message type.
     * 
     * Note that this id is not unique among messages of same type.
     */
    const msgid_t id;

protected:
    /**
     * @brief Construct a new Message object.
     * 
     * Prevent direct instantiation of Message base class.
     */
    explicit Message(const msgid_t id);

    /**
     * @brief Destroy the Message object.
     */
    virtual ~Message() = default;
};

} // namespace EBPS