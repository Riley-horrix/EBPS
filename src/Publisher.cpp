/**
 * @file Publisher.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Declaration of Publisher Class
 * @version 0.1
 * @date 2024-11-11
 */
#include "Publisher.h"

#include "Message.h"

#include <iostream>
#include <memory>

using namespace EBPS;

Publisher::Publisher() {}

void Publisher::connectVendor(MessageVendor& vendor) {
    vendors.push_back(vendor);
}

void Publisher::publish(std::shared_ptr<Message> message) {
    for (MessageVendor& vendor : vendors) {
        vendor.post(message);
    }
}
