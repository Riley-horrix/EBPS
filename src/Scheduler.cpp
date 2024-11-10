/**
 * @file Scheduler.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of a Task Scheduler
 * @version 0.1
 * @date 2024-11-09
 */
#include "Scheduler.h"

#include "TimeUtils.h"

#include <functional>
#include <thread>
#include <forward_list>

using namespace EBPS;

// TODO :: extract queue manipulation to inline functions

Scheduler::Handle::Handle(std::function<void(void)> function):
    cancelFn(std::move(function)) {}

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

    insertTask(taskToQueue);

    // Return a cancellable handle
    return std::make_unique<Handle>([this, taskToQueue]() {
        this->cancelTask(taskToQueue.uid);
    });
}

std::unique_ptr<Scheduler::Handle>
Scheduler::interval(std::function<void(void)> task, time_t period) {
    // Immediately queue task
    time_t timeToQueue = TimeUtils::now();
    TimedTask taskToQueue(task, timeToQueue, period);

    insertTask(taskToQueue);

    // Return a cancellable handle
    return std::make_unique<Handle>([this, taskToQueue]() {
        this->cancelTask(taskToQueue.uid);
    });
}


// TODO :: make this use std::shared_ptr? it should probably
// get front and dequeue at the same time
void Scheduler::start(void) {
    shouldRun = true;
    while (shouldRun && !queue.empty()) {
        // Reset the cancelledTask flag
        cancelledTask = false;

        // Current time in milliseconds
        time_t timeNow = TimeUtils::now();

        // Get the next task to run
        TimedTask& task = queue.front();

        if (timeNow < task.time) {
            // Sleep the current thread until the next task
            std::this_thread::sleep_for(
                std::chrono::milliseconds(task.time - timeNow)
            );

            timeNow = TimeUtils::now();
        }

        // Execute task and handle requeuing or cancelling
        task.task();

        // Remove task from the queue
        if (task.interval == NO_REPEAT || cancelledTask) {
            queue.pop_front();
        } else {
            // Else requeue it at the correct position
            // Use time now rather than task time to prevent backlog of tasks
            task.time = timeNow + task.interval;
            insertTask(task);
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

void Scheduler::insertTask(const TimedTask& task) {
    auto it = queue.begin();
    auto backIt = queue.before_begin();

    // Find the first task with a larger timestamp than the task to queue.
    // You need 2 iterators here since the queue is only forward directional.
    while (it != queue.end() && it->time < task.time) {
        it++;
        backIt++;
    }

    // If the element should be inserted at front then push front
    if (backIt == queue.before_begin()) {
        queue.push_front(task);
    } else {
        // Else emplace after the back iterator
        queue.emplace_after(backIt, task);
    }
}