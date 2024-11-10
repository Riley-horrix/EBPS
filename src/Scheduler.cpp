/**
 * @file Scheduler.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of a Task Scheduler
 * @version 0.1
 * @date 2024-11-09
 */
#include "Scheduler.h"

#include "TimeUtils.h"

#include <iostream>
#include <functional>
#include <thread>
#include <forward_list>

using namespace EBPS;

// TODO :: extract queue manipulation to inline functions

Scheduler::Handle::Handle(std::function<void(void)> function) : cancelFn(std::move(function)) {}

Scheduler::Handle::~Handle() {}

void Scheduler::Handle::cancel(void) {
    // Execute cancel function and then make the function do nothing.
    cancelFn();
    cancelFn = []() {};
}

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

std::unique_ptr<Scheduler::Handle>
Scheduler::timeout(std::function<void(void)> task, 
    time_t timeout) {

    time_t timeToQueue = timeout + TimeUtils::now();

    TimedTask taskToQueue(task, timeToQueue);

    // Find the first task with a larger timestamp than the task to queue.
    // You need 2 iterators here since the queue is only forward directional.
    auto it = queue.begin();
    auto backIt = queue.before_begin();
    while (it != queue.end() && it->time < timeToQueue) {
        it++;
        backIt++;
    }

    // Insert element
    queue.emplace_after(backIt, taskToQueue);

    // Return a cancellable handle
    return std::make_unique<Handle>([this, taskToQueue]()
                                    { this->cancelTask(taskToQueue.uid); });
}

std::unique_ptr<Scheduler::Handle>
Scheduler::interval(std::function<void(void)> task, 
    time_t period) {

    time_t timeToQueue = TimeUtils::now();

    TimedTask taskToQueue(task, timeToQueue, period);

    // Find the first task with a larger timestamp than the task to queue.
    // You need 2 iterators here since the queue is only forward directional.
    auto it = queue.begin();
    auto backIt = queue.before_begin();
    while (it != queue.end() && it->time < timeToQueue) {
        it++;
        backIt++;
    }

    // Insert element
    queue.emplace_after(backIt, taskToQueue);

    // Return a cancellable handle
    return std::make_unique<Handle>([this, taskToQueue]()
                                    { this->cancelTask(taskToQueue.uid); });
}


// TODO :: make this use std::shared_ptr? it should probably
// get front and dequeue at the same time
void Scheduler::start(void) {
    shouldRun = true;
    while (shouldRun && !queue.empty()) {
        // Reset the cancelledTask flag
        cancelledTask = false;

        // Current time in microseconds
        time_t timeNow = TimeUtils::now();

        // Get the next task to run
        auto& task = queue.front();

        if (timeNow < task.time) {
            // Sleep the current thread until the next task
            std::this_thread::sleep_for(std::chrono::microseconds(task.time - timeNow));

            timeNow = TimeUtils::now();
        }

        // Execute task and handle requeuing or cancelling
        task.task();

        // Remove task from the queue

        if (task.interval == NO_REPEAT || cancelledTask) {
            queue.pop_front();
        // If task is set to repeat and is not cancelled then reschedule
        } else {

            // std::cout << "requeue\n";
            // Else requeue it at the correct position
            task.time = timeNow + task.interval;

            auto it = queue.begin();
            auto backIt = queue.before_begin();
            while (it != queue.end() && it->time < task.time) {
                it++;
                backIt++;
            }

            queue.emplace_after(backIt, task);
            queue.pop_front();
        }        
    }
}

void Scheduler::stop(void) {
    shouldRun = false;
}

void Scheduler::cancelTask(const uid_t uid) {
    // Find task with specified uid
    auto it = queue.begin();
    auto backIt = queue.before_begin();
    while (it != queue.end() && it->uid != uid) {
        it++; 
        backIt++;
    }

    if (backIt == queue.before_begin()) {
        // If the task to cancel is the task currently running then 
        // set cancelled.
        cancelledTask = true;
    } else {
        // Else erase the task
        queue.erase_after(backIt);
    }
}

Scheduler::TimedTask::TimedTask(const std::function<void(void)> task, 
    const time_t timestamp, const time_t interval): 
        uid(0), time(timestamp), task(task), interval(interval) {
    static uid_t uidGen = 0;
    uid = uidGen++;
}
