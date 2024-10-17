#include "Circuit.h"
#include "../Mediator.h"

void Circuit::activate() {
    std::cout << name << " circuit activated." << std::endl;
    mediator->notify(name, "CircuitActivated");
}