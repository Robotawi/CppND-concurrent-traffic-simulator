# CPPND: Program a Concurrent Traffic Simulation

In this project, the given is a traffic simulator in which vehicles are crossing intersections one by one. A traffic light system is to be adopted at the intersection. The aim is to develop the traffic light class and establish a thread safe communication between the vehicles, the intersections, and the traffic lights. This project is designed to apply the concepts of concurrency in C++ (such as mutexes, locks and message queues) through Udacity's C++ Nanodegree.

# The given traffic simulator
<img src="data/traffic_simulation.gif"/>

With the increasing traffic in the city, traffic lights are needed for road safety. Each intersection will therefore be equipped with a traffic light. When the project is built initially, all traffic lights will be green. When you are finished with the project, your traffic simulation should run with red lights controlling traffic, just as in the .gif file above. In this project, you will build a suitable and thread-safe communication protocol between vehicles and intersections to complete the simulation. Use your knowledge of concurrent programming to implement the traffic lights and integrate them properly in the code base.

## Installation 
- #### Dependencies for Running Locally
  * cmake >= 2.8
    * All OSes: [click here for installation instructions](https://cmake.org/install/)
  * make >= 4.1 (Linux, Mac), 3.81 (Windows)
    * Linux: make is installed by default on most Linux distros
    * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
    * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
  * OpenCV >= 4.1
    * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
  * gcc/g++ >= 5.4
    * Linux: gcc / g++ is installed by default on most Linux distros
    * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
    * Windows: recommend using [MinGW](http://www.mingw.org/)

- #### Clone
    ```
    git clone https://github.com/Robotawi/CppND-concurrent-traffic-simulator.git
    ```

- #### Setup
  ```
  cd CppND-Memory-Management-ChatBot
  mkdir build 
  cd build
  cmake ..
  make
  ```

## Running
```
./traffic_simulation
```

## The Implemented Changes to adapt traffic lights and concurrency

### 1 :
I defined a class `TrafficLight` as a child class of `TrafficObject`. The class has public methods `void waitForGreen()` and `void simulate()` as well as `TrafficLightPhase getCurrentPhase()`, where `TrafficLightPhase` is an enum that can be either `red` or `green`. Also, I added the private method `void cycleThroughPhases()`, and the private member `_currentPhase` which can take `red` or `green` as its value to let the check for the traffic happen later.

### 2 :
I implemented the function `void cycleThroughPhases()` with an infinite loop that measures the time between two loop cycles and toggles the current phase of the traffic light between red and green and sends an update method to the message queue using move semantics. The cycle duration is a random value between 4 and 6 seconds. In the while-loop, `std::this_thread::sleep_` to wait 1ms between two cycles. Finally, the method `cycleThroughPhases` is started in a thread when the public method `simulate` is called.

### 3 :
A class `MessageQueue` is defined with public methods send and receive. Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. The class defines an `std::dequeue` called `_queue`, which stores objects of type `TrafficLightPhase`. There is a `std::condition_variable` as well as an `std::mutex` defined as private members.

### 4 :
The implementation of the method `Send`, uses the mechanisms `std::lock_guard<std::mutex>` as well as `_condition.notify_one()` to add a new message to the queue and afterwards send a notification. In class `TrafficLight`, create a private member of type `MessageQueue` for messages of type `TrafficLightPhase` and use it within the infinite loop to push each new `TrafficLightPhase` into it by calling send in conjunction with move semantics.

### 5 :
The method `Receive` uses `std::unique_lock<std::mutex>` and `_condition.wait()` to wait for and receive new messages and pull them from the queue using move semantics. The received object is returned by the receive function. The implementation of the method `waitForGreen` is added and within it, an infinite while-loop runs and repeatedly calls the `receive` function on the message queue. Once it receives `TrafficLightPhase::green`, the method returns.

### 6 :
In class Intersection, a private member `_trafficLight` of type `TrafficLight` is added. In method `Intersection::simulate()`, start the simulation of `_trafficLight`. Then, in the method `Intersection::addVehicleToQueue`, the methods `TrafficLight::getCurrentPhase` and `TrafficLight::waitForGreen` are used to block the execution until the traffic light turns green.

## Contact
If you are interested in the presented work/ideas or if you have any questions, you are welcome to connect with me on [LinkedIn](https://www.linkedin.com/in/mohraess). We can discuss about this project and other interesting projects.