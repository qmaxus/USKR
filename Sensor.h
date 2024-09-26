#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>

class Mediator; // Предварительное объявление

class Sensor {
private:
    Mediator* mediator;
    std::string name;
public:
    Sensor(Mediator* med, const std::string& n) : mediator(med), name(n) {}

    void triggerAlarm();
};

#endif // SENSOR_H
