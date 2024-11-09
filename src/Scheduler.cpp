/**
 * @file Scheduler.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of the Task Scheduler
 * @version 0.1
 * @date 2024-11-09
 */
#include "Scheduler.h"

#include <functional>

using namespace EBPS;

Scheduler::Handle::Handle(std::function<void(void)> function):
    cancelFn(std::move(function)) {}

Scheduler::Handle::~Handle() {}

void Scheduler::Handle::cancel(void) {
    // Execute cancel function and then make the function do nothing.
    cancelFn();
    cancelFn = [](){};
}

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

std::unique_ptr<Scheduler::Handle>
EBPS::Scheduler::createTimeout(std::function<void(void)> task,
    uint64_t timeout) {
    
    // Use clock here
    uint64_t timeToQueue = timeout; // + Time::now()

    TimedTask taskToQueue(task, timeToQueue);

    // Find the first task with a larger timestamp than the task to queue.
    // You need 2 iterators here since the queue is only forward directional.
    auto it = queue.begin();
    auto backIt = queue.begin();
    while (it++->time < timeToQueue) {
        backIt++;
    }

    queue.emplace_after(backIt, taskToQueue);

    return std::make_unique<Handle>([this, taskToQueue]() {
        this->cancelTask(taskToQueue.uid);
    });
}

void EBPS::Scheduler::start(void) {}

void EBPS::Scheduler::stop(void) {}

void EBPS::Scheduler::cancelTask(const uint64_t uid) {
    auto it = queue.begin();
    auto backIt = queue.begin();
    while (it++->uid != uid) {
        backIt++;
    }

    queue.erase_after(backIt);
}

EBPS::Scheduler::TimedTask::TimedTask(const std::function<void(void)> task,
    const uint64_t timestamp, const uint64_t interval): 
        uid(0), time(timestamp), task(task), interval(interval) {}
