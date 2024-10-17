#include "Laser.h"
#include "../Mediator.h"

void Laser::fire() {
    std::cout << name << " laser fired!" << std::endl;
    mediator->notify(name, "LaserFired");
}