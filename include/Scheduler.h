/**
 * @file Scheduler.h
 * @author Riley Horrix (riley@horrix.com)
 * @brief Definition of a task scheduler.
 * @version 0.1
 * @date 2024-11-09
 *
 */
#pragma once

#include "TimeUtils.h"

#include <forward_list>
#include <functional>
#include <memory>
#include <cstdint>

namespace EBPS {

class Scheduler {
public:
    /**
     * @brief Simple handle class that allows users of the scheduler to cancel
     * tasks at any point, using the cancel function.
     */
    class Handle
    {
    public:
        /**
         * @brief Construct a new Handle object.
         *
         * @param cancelFn The backing function that will cancel the task.
         */
        Handle(std::function<void(void)> cancelFn);

        /**
         * @brief Destroy the Handle object
         */
        ~Handle();

        /**
         * @brief Cancel the interval / timeout created from this handle.
         *
         * This can be called while the function that created this timeout
         * is executing to cancel the the currently running task.
         */
        void cancel(void);

    private:
        /**
         * @brief The backing function object that cancels the task that
         * created this handle.
         */
        std::function<void(void)> cancelFn;
    };

    /**
     * @brief Construct a new Scheduler object.
     */
    Scheduler();

    /**
     * @brief Destroy the Scheduler object.
     */
    ~Scheduler();

    /**
     * @brief Schedules a task to be executed after a defined amount of time
     * has passed.
     *
     * @param task A void function object containing the task executable.
     * @param timeout The number of microseconds required before executing
     *      the task.
     * @return std::unique_ptr<Handle> A pointer to a handle object that allows
     *      the caller to cancel the task.
     */
    std::unique_ptr<Handle> timeout(std::function<void(void)> task, time_t timeout);

    /**
     * @brief Schedule a task to be executed every PERIOD number of milliseconds.
     * 
     * @param task A void function object containing the task executable.
     * @param period The number of microseconds required between executing
     *      the task.
     * @return std::unique_ptr<Handle> A pointer to a handle object that allows
     *      the caller to cancel the repeating task.
     */
    std::unique_ptr<Handle> interval(std::function<void(void)> task, time_t period);

    /**
     * @brief Starts the scheduler.
     *
     * This function is blocking and will run the scheduler until there are no
     * more tasks left in the queue.
     *
     * Ensure that you have already queued some tasks before calling this
     * method.
     */
    void start(void);

    /**
     * @brief Stops the scheduler.
     *
     * This is a non-destructive call to halt the scheduler, as such the task
     * queue will be maintained and it can be started again.
     */
    void stop(void);

    //! @brief Represents a non repeating interval
    static const time_t NO_REPEAT = -1;

private:

    using uid_t = uint32_t;

    /**
     * @brief Cancels a task given its unique identifier.
     *
     * @param uid Unique identifier.
     */
    void cancelTask(const uid_t uid);

    /**
     * @brief Internal representation of a task object;
     *
     * Time field allows tasks to be ordered by time until execution.
     */
    struct TimedTask
    {
        TimedTask(const std::function<void(void)> task, const time_t timestamp, const time_t interval = Scheduler::NO_REPEAT);

        //! @brief Unique Identifier - used to cancel tasks.
        uid_t uid;

        //! @brief Timestamp for required execution time in microseconds.
        time_t time;

        //! @brief The actual payload.
        const std::function<void(void)> task;

        /**
         * @brief The repeat interval of this task in microseconds.
         *
         * If this number is set to Scheduler::NO_REPEAT then the task will
         * not repeat.
         */
        const time_t interval;
    };

    // This could be implemented better as a min-heap in an array
    //! @brief The backing class containing the tasks
    std::forward_list<TimedTask> queue;

    //! @brief If a task has been cancelled during execution of a task.
    bool cancelledTask = false;

    //! @brief If the scheduler should continue to run
    bool shouldRun = false;
};

} // Namespace EBPS