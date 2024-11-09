/**
 * @file Scheduler.cpp
 * @author Riley Horrix (riley@horrix.com)
 * @brief Implementation of a Task Scheduler
 * @version 0.1
 * @date 2024-11-09
 */
#include "Scheduler.h"

#include "Time.h"

#include <functional>
#include <thread>

using namespace EBPS;

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
EBPS::Scheduler::createTimeout(std::function<void(void)> task, 
    uint64_t timeout) {

    // Use clock here
    uint64_t timeToQueue = timeout + Time::now();

    TimedTask taskToQueue(task, timeToQueue);

    // Find the first task with a larger timestamp than the task to queue.
    // You need 2 iterators here since the queue is only forward directional.
    auto it = queue.begin();
    auto backIt = queue.begin();
    while (it++->time < timeToQueue) {
        backIt++;
    }

    // Insert element
    queue.emplace_after(backIt, taskToQueue);

    // Return a cancellable handle
    return std::make_unique<Handle>([this, taskToQueue]()
                                    { this->cancelTask(taskToQueue.uid); });
}

void EBPS::Scheduler::start(void) {
    while (shouldRun && !queue.empty()) {
        // Reset the cancelledTask flag
        cancelledTask = false;

        // Current time in microseconds
        uint64_t timeNow = Time::now();

        // Get the next task to run
        auto& task = queue.front();

        if (timeNow < task.time) {
            // Sleep the current thread until the next task
            std::this_thread::sleep_for(std::chrono::microseconds(task.time - timeNow));

            timeNow = Time::now();
        }
        
        // Execute task and handle requeuing or cancelling
        task.task();

        if (task.interval == NO_REPEAT || cancelledTask) {
            // If task has been cancelled or isnt repeating then remove it
            queue.pop_front();
        } else {
            // Else requeue it at the correct position
            task.time = timeNow + task.interval;

            auto it = queue.begin();
            auto backIt = queue.begin();
            while (it++->time < task.time) {
                backIt++;
            }

            // Insert element
            queue.emplace_after(backIt, task);
        }
        
    }
}

void EBPS::Scheduler::stop(void) {
    shouldRun = false;
}

void EBPS::Scheduler::cancelTask(const uint64_t uid)
{
    // Find task with specified uid
    auto it = queue.begin();
    auto backIt = queue.begin();
    while (it++->uid != uid) {
        backIt++;
    }

    if (backIt == queue.begin()) {
        // If the task to cancel is the task currently running then pop
        // the task and set cancelled.
        queue.pop_front();
        cancelledTask = true;
    } else {
        // Else erase the task
        queue.erase_after(backIt);
    }
}

EBPS::Scheduler::TimedTask::TimedTask(const std::function<void(void)> task, 
    const uint64_t timestamp, const uint64_t interval): 
        uid(0), time(timestamp), task(task), interval(interval) {
    static uint64_t uidGen = 0;
    uid = uidGen++;
}
