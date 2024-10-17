#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <string>

class Mediator; // Предварительное объявление

class Circuit {
private:
    Mediator* mediator;
    std::string name;
public:
    Circuit(Mediator* med, const std::string& n) : mediator(med), name(n) {}

    void activate();
};

#endif // CIRCUIT_H