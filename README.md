# EBPS
A simple event-based publish-subscribe messaging model, this was designed for use as part of [Imperial College Rocketry Firmware](https://github.com/icl-rocketry/Ricardo-PickleRick).

## Building

This project uses cmake, and catch2 for testing.

To make the library, call
```
make
```

To make the library and run the test cases, run 
```
make test
```

Catch 2 is automatically detected and loaded if your system does not have it.

## What is it?

It defines a few major components.

### Scheduler

This is the main component of an event based message passing system, it enables classes to schedule repeating and timed out tasks to execute 'asynchronously'.

Classically, there should be a global instance of the scheduler that all classes can access and queue tasks on.

This could (for example) be used by a hardware sensor to set a polling function that runs at a certain frequency.

### Messaging System

This system defines some important base classes,
- MessageVendor
- Subscriber
- Publisher
- Message

These can be used to create a completely decoupled software application, where the components only interface through messages; this massively helps with scalability of the software.

To become a subscriber, you just have to get your class to declare a list of desired topics and derive from the Subscriber class and extend the ```notify()``` function. This allows this message to receive messages.

To become a publisher, you just have to derive from Publisher. This gives you access to the ```publish()``` function, allowing you to emit a message.

The MessageVendor connects the Publishers to Subscribers and allows automatic message transmission.

### Time Utils

These utilities are essential for the time scheduling.