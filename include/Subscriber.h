/**
 * @file Publisher.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of the Subscriber Class
 * @version 0.1
 * @date 2024-11-11
 */
#pragma once

#include "Message.h"

#include <memory>
#include <vector>

namespace EBPS {

class Subscriber {
public:
    /**
     * @brief Construct a new Subscriber object.
     */
    Subscriber();

    /**
     * @brief Destroy the Subscriber object.
     * 
     * Virtual to allow polymorphism
     */
    virtual ~Subscriber() = default;

    /**
     * @brief Send this subscriber a message.
     * 
     * @param message Message to send.
     */
    virtual void notify(std::shared_ptr<Message> message) = 0;

    /**
     * @brief Pre-determined list of messages that this class consumes.
     */
    std::vector<msgid_t> messagesConsumed;
private:
protected:
};

} // Namespace EBPS