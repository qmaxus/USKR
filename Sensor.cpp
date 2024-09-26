#include "Sensor.h"
#include "Mediator.h"

void Sensor::triggerAlarm() {
    std::cout << name << " triggered an alarm." << std::endl;
    mediator->notify(name, "AlarmTriggered");
}