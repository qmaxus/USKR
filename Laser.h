#ifndef LASER_H
#define LASER_H

#include <iostream>
#include <string>

class Mediator; // Предварительное объявление

class Laser {
private:
    Mediator* mediator;
    std::string name;
public:
    Laser(Mediator* med, const std::string& n) : mediator(med), name(n) {}

    void fire();
};

#endif // LASER_H