/**
 * @file MessageVendor.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Declaration of MessageVendor class
 * @version 0.1
 * @date 2024-11-11
 */
#include "MessageVendor.h"
#include "Publisher.h"

#include <iostream>
#include <memory>
#include <functional>

using namespace EBPS;

MessageVendor::MessageVendor() {}

MessageVendor::~MessageVendor() {}

void MessageVendor::post(std::shared_ptr<Message> message) {
    msgid_t msg = message->id;
    for (Subscriber& sub : subscriberMap[msg]) {
        sub.notify(message);
    }
}

void MessageVendor::addPublisher(Publisher &publisher) {
    publisher.connectVendor(*this);
}

void MessageVendor::addSubscriber(Subscriber &subscriber) {
    for (msgid_t msg : subscriber.messagesConsumed) {
        if (subscriberMap.contains(msg)) {
            subscriberMap[msg].push_back(subscriber);
        } else {
            std::vector<std::reference_wrapper<Subscriber>> vec;
            vec.push_back(subscriber);
            subscriberMap[msg] = vec;
        }
    }
}
