/**
 * @file MessageVendor.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of the MessageVendor class
 * @version 0.1
 * @date 2024-11-11
 */
#pragma once

#include "Message.h"
#include "Subscriber.h"

#include <vector>
#include <memory>
#include <map>
#include <functional>

namespace EBPS {

// Forward declarations
class Publisher;
class Subscriber;

/**
 * @brief MessageVendor class implements a basic message passing system,
 * passing messages automatically from publishers to subscribers.
 * 
 * Note that this class (and publishers / subscribers) use references rather
 * than smart pointers because the scope of the subscribers / publishers will
 * (for now) always occur during the scope of this message vendor.
 * 
 * If this changes, the references should probably be changed to shared ptrs. 
 */
class MessageVendor {
public:
    /**
     * @brief Construct a new Message Vendor object.
     */
    MessageVendor();

    /**
     * @brief Destroy the Message Vendor object.
     */
    ~MessageVendor();

    /**
     * @brief Post a message to the vendor.
     * 
     * This function will then automatically pass this message into all of 
     * the subscribed classes, listening to the ID of this message.
     * 
     * @param message The message to send.
     */
    void post(std::shared_ptr<Message> message);

    /**
     * @brief Connect a publisher to publish to this vendor service.
     * 
     * Internally this will register the vendor inside the publisher.
     * 
     * @param publisher The publisher to connect.
     */
    void addPublisher(Publisher& publisher);

    /**
     * @brief Connect a subscriber to receive messages from this vendor.
     * 
     * @param subscriber The Subscriber to add.
     */
    void addSubscriber(Subscriber& subscriber);
    
    // void addPublisherSubscriber(const PublisherSubscriber& pubsub);
private:
    /**
     * @brief Map from message ids to the subscribers listening to them.
     */
    std::map<msgid_t, std::vector<std::reference_wrapper<Subscriber>>> subscriberMap;
};

} // Namespace EBPS