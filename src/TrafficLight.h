#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

enum TrafficLightPhase{ red, green};

class Vehicle;

template <class T>
class MessageQueue
{
public:
    void send (T && tlp);
    T receive();
private:
    std::deque<T> _queue;
    std::condition_variable _cond;
    std::mutex _mtx;
};

class TrafficLight:public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    // getters / setters
    TrafficLightPhase getCurrentPhase();
    // typical behaviour methods
    void waitForGreen();
    void simulate();
private:
    // typical behaviour methods
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;

    MessageQueue<TrafficLightPhase> _msgqueue;

    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif