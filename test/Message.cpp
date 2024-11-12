/**
 * @file Message.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of tests for Messages, Subscriber, Publisher
 * and Vendor classes.
 * @version 0.1
 * @date 2024-11-11
 */
#include "Message.h"
#include "MessageVendor.h"
#include "Subscriber.h"
#include "Publisher.h"
#include "Scheduler.h"

#include <catch2/catch_all.hpp>
#include <iostream>

using namespace EBPS;

enum MessageIds {
    GYRO = 1
};

// Uses a globally defined scheduler
// This needs to be reset every test
Scheduler SCHEDULER;

class GyroMessage : public Message {
public:
    GyroMessage(double accX, double accY, double accZ): Message(GyroMessage::id), accX(accX), accY(accY), accZ(accZ) {}
    
    ~GyroMessage() {}

    double accX = 0;
    
    double accY = 0;
    
    double accZ = 0;

    static const msgid_t id = GYRO;
};

// Static storage for the message id
const msgid_t GyroMessage::id;

class GyroSensor : public Publisher {
public:
    GyroSensor() {
        gyroMsg = std::make_shared<GyroMessage>(0, 0, 0);

        // Setup interval poll.
        auto handle = SCHEDULER.interval([this]() {
            this->poll();
        }, std::ceil(1000 / frequency));

        pollHandle = std::move(handle);
    }

    ~GyroSensor() {}

    // Read from sensors and emit message
    void poll() {
        gyroMsg->accX = 0.111;
        gyroMsg->accY = 0.947;
        gyroMsg->accZ = 1.2324;

        publish(gyroMsg);
    }

private:
    // Hz
    double frequency = 1000;

    std::shared_ptr<GyroMessage> gyroMsg;

    std::unique_ptr<EBPS::Scheduler::Handle> pollHandle;
};

class Estimator : public Subscriber {
public:
    Estimator() {

        messagesConsumed.push_back(GyroMessage::id);

        auto handle = SCHEDULER.interval([this]() {
            this->update();
        }, std::ceil(1000 / frequency));

        pollHandle = std::move(handle);
    }

    ~Estimator() {}

    void notify(std::shared_ptr<Message> message) {
        msgid_t id = message->id;

        if (id == GyroMessage::id) {
            std::shared_ptr<GyroMessage> gyrMsg = std::static_pointer_cast<GyroMessage>(message);

            accX = gyrMsg->accX;
            accY = gyrMsg->accY;
            accZ = gyrMsg->accZ;
        }
    }

    void update() {
        // Use accX accY accZ here
        receivedMessage = accX == 0.111 && accY == 0.947 && accZ == 1.2324;
    }

    bool receivedMessage = false;

private:
    double frequency = 500;

    double accX = 0;
    double accY = 0;
    double accZ = 0;

    std::unique_ptr<EBPS::Scheduler::Handle> pollHandle;
};

TEST_CASE("Messaging system can pass messages between classes", "[Message]") {
    MessageVendor vendor;

    GyroSensor gyro;
    Estimator estimator;

    std::cout << "[Message] system tests\n";

    vendor.addPublisher(gyro);
    vendor.addSubscriber(estimator);

    int count = 0;

    std::unique_ptr<Scheduler::Handle> handle = SCHEDULER.interval([&]() {
        count++;
        if (estimator.receivedMessage || count >= 20) {
            SCHEDULER.clear();
            handle->cancel();
        }
    }, 1);

    SCHEDULER.start();

    // Require that the estimator receives messages before count >= 20
    REQUIRE(count > 0); 
    REQUIRE(count < 20);
}
