/**
 * @file Publisher.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of the Publisher Class
 * @version 0.1
 * @date 2024-11-11
 */
#pragma once

#include "Message.h"
#include "MessageVendor.h"

#include <vector>
#include <memory>
#include <functional>

namespace EBPS {

// Forward declarations
class MessageVendor;

class Publisher {
public:
    /**
     * @brief Construct a new Publisher object.
     */
    Publisher();

    /**
     * @brief Destroy the Publisher object.
     * 
     * Virtual to allow polymorphism
     */
    virtual ~Publisher() = default;

    /**
     * @brief Setup a vendor to receive messages from this Publisher.
     * 
     * @param vendor The vendor to connect
     */
    void connectVendor(MessageVendor& vendor);

private:
    //! @brief Collection of the vendors this publisher is connected to
    std::vector<std::reference_wrapper<MessageVendor>> vendors;

protected:
    /**
     * @brief Publish a message to any vendors that are connected to this
     * publisher.
     * 
     * @param message The message to send.
     */
    void publish(std::shared_ptr<Message> message);
};

} // Namespace EBPS