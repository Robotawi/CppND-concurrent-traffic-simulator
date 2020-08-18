#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <future>
#include <vector>


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> _ulck(_mtx);
    _cond.wait(_ulck,[this]{return !_queue.empty();});
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lck(_mtx);
    _queue.emplace_back(std::move(msg));
    _cond.notify_one();
}


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while(true){
        if (_msgqueue.receive() == TrafficLightPhase::green){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));

}

void TrafficLight::cycleThroughPhases()
{
    std::random_device rdev;
    while(true){
        std::mt19937 genseed(rdev());
        std::uniform_int_distribution<> dis_range(4, 6);
        auto randy = dis_range(genseed);
        std::this_thread::sleep_for(std::chrono::seconds(randy));

        if (_currentPhase == TrafficLightPhase::green){
            _currentPhase = TrafficLightPhase::red;
        }
        else{
            _currentPhase = TrafficLightPhase::green;
        }
        _msgqueue.send(std::move(_currentPhase));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}